#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <signal.h>
#ifdef WIN32
#include <conio.h>
#else
/**
 Linux (POSIX) implementation of _kbhit().
 Morgan McGuire, morgan@cs.brown.edu
 */
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#ifndef __GNUC__
 #include <stropts.h>
#endif
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#endif

#include <os/OsIntTypes.h>
#include <os/OsSysLog.h>

#include <recon/ReconSubsystem.hxx>
#include <recon/SipXHelper.hxx>

#include "reConServerConfig.h"
#include "reConServerProcess.h"
#include "reConServer.h"
#include "MyMessageDecorator.h"
#include "MyConversationManager.h"
//#include "B2BCallManager.h"
#include "UserAgent.h"
#include "WebAdmin.h"
#include "WebAdminThread.h"
#include "ConversationManagerSettings.h"

#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>
#include <rutil/DnsUtil.hxx>
#include <rutil/BaseException.hxx>
#include <rutil/WinLeakCheck.hxx>

using namespace recon;
using namespace resip;
using namespace std;

#define RESIPROCATE_SUBSYSTEM ReconSubsystem::RECON

void sleepSeconds(unsigned int seconds)
{
#ifdef WIN32
   Sleep(seconds*1000);
#else
   sleep(seconds);
#endif
}

static bool finished = false;

static void
signalHandler(int signo)
{
   std::cerr << "Shutting down" << endl;
   finished = true;
}


namespace recon
{

ReConServerProcess::ReConServerProcess() :
   mWebAdminV4(0),
   mWebAdminV6(0),
   mWebAdminThread(0)
{
}

ReConServerProcess::~ReConServerProcess()
{
   if(mWebAdminThread) delete mWebAdminThread;
   if(mWebAdminV4) delete mWebAdminV4;
   if(mWebAdminV6) delete mWebAdminV6;
}

int 
ReConServerProcess::main (int argc, char** argv)
{
#ifndef _WIN32
   if ( signal( SIGPIPE, SIG_IGN) == SIG_ERR)
   {
      cerr << "Couldn't install signal handler for SIGPIPE" << endl;
      exit(-1);
   }
#endif

#if defined(WIN32) && defined(_DEBUG) && defined(LEAK_CHECK) 
   resip::FindMemoryLeaks fml;
   {
#endif

   if ( signal( SIGINT, signalHandler ) == SIG_ERR )
   {
      cerr << "Couldn't install signal handler for SIGINT" << endl;
      exit( -1 );
   }

   if ( signal( SIGTERM, signalHandler ) == SIG_ERR )
   {
      cerr << "Couldn't install signal handler for SIGTERM" << endl;
      exit( -1 );
   }

   Data defaultConfigFilename("reConServer.config");
   ReConServerConfig reConServerConfig;
   try
   {
      reConServerConfig.parseConfig(argc, argv, defaultConfigFilename);
   }
   catch(std::exception& e)
   {
      ErrLog(<< "Exception parsing configuration: " << e.what());
      exit(-1);
   }

   Data runAsUser = reConServerConfig.getConfigData("RunAsUser", "", true);
   Data runAsGroup = reConServerConfig.getConfigData("RunAsGroup", "", true);
   Data pidFile = reConServerConfig.getConfigData("PidFile", "", true);
   bool daemonize = reConServerConfig.getConfigBool("Daemonize", false);
   bool keyboardInput = reConServerConfig.getConfigBool("KeyboardInput", !daemonize);
   if(daemonize && keyboardInput)
   {
      ErrLog(<< "Ignoring KeyboardInput=true setting as we are running as a daemon");
      keyboardInput = false;
   }
   setPidFile(pidFile);
   // Daemonize if necessary
   if(daemonize)
   {
      ReConServerProcess::daemonize();
   }

   //////////////////////////////////////////////////////////////////////////////
   // Create ConversationManager and UserAgent
   //////////////////////////////////////////////////////////////////////////////
   {
      std::auto_ptr<reConServer> server(new reConServer);

      // Create Http Server
      //if(mHttpPort != 0)
      {
         Data httpAuthPwd("Buddy154");
         std::list<clicktocall::WebAdmin*> webAdminList;
         mWebAdminV4 = new clicktocall::WebAdmin(*server, false /* web challenge */, Data::Empty, httpAuthPwd, 5081, V4);
         webAdminList.push_back(mWebAdminV4);
         //if(mIsV6Avail)
         {
            //mWebAdminV6 = new clicktocall::WebAdmin(*myConversationManager, false /* web challenge */, Data::Empty, httpAuthPwd, 5081, V6);
            //webAdminList.push_back(mWebAdminV6);
         }
         mWebAdminThread = new clicktocall::WebAdminThread(webAdminList);
      }

      //////////////////////////////////////////////////////////////////////////////
      // Startup and run...
      //////////////////////////////////////////////////////////////////////////////

      server->startup(reConServerConfig);
      if(mWebAdminThread) mWebAdminThread->run();

      // Drop privileges (can do this now that sockets are bound)
      if(!runAsUser.empty())
      {
         InfoLog( << "Trying to drop privileges, configured uid = " << runAsUser << " gid = " << runAsGroup);
         dropPrivileges(runAsUser, runAsGroup);
      }

      int input;
      while(true)
      {
         server->process(50);
         if(keyboardInput)
         {
            while(_kbhit() != 0)
            {
#ifdef WIN32
               input = _getch();
               //processKeyboard(input, *myConversationManager, ua);
#else
               input = fgetc(stdin);
               fflush(stdin);
               //cout << "input: " << input << endl;
               //processKeyboard(input, *myConversationManager, ua);
#endif
            }
         }
         if(finished) break;
      }

      server->shutdown();
      if(mWebAdminThread) mWebAdminThread->shutdown();
      if(mWebAdminThread) mWebAdminThread->join();
   }
   InfoLog(<< "reConServer is shutdown.");
   OsSysLog::shutdown();
   ::sleepSeconds(2);

#if defined(WIN32) && defined(_DEBUG) && defined(LEAK_CHECK) 
} // end FML scope
#endif
}

} // namespace recon

/* ====================================================================

 Copyright (c) 2007-2008, Plantronics, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are 
 met:

 1. Redistributions of source code must retain the above copyright 
    notice, this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution. 

 3. Neither the name of Plantronics nor the names of its contributors 
    may be used to endorse or promote products derived from this 
    software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ==================================================================== */
