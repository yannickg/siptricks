#if defined(HAVE_CONFIG_HXX)
#include "resip/stack/config.hxx"
#endif

#include <cstring>
#include <cassert>

#ifndef __APPLE__
bool TRUE=true;
bool FALSE=false;
#endif

#include "CallFactory.h"
#include "siplayer/UaThread.h"
#include "helper/MessageTransport.h"
#include "Callback.h"
#include "cnode/Client.h"
#include "cnode/Properties.h"

#include <rutil/Logger.hxx>
#include <rutil/BaseException.hxx>

#ifdef USE_SSL
#include <resip/stack/ssl/Security.hxx>
#endif

#include <signal.h>

using namespace resip;
using namespace std;
using namespace siptricks;

#define RESIPROCATE_SUBSYSTEM Subsystem::APP

siptricks::UaThread* uaThread = NULL;
messaging::MessageTransport* msgTransport = NULL;

static void
signalHandler(int signo)
{
  uaThread->shutdown();
  msgTransport->shutdown();
}

typedef unsigned char byte;

int main(int argc, char* argv[])
{
  #ifndef _WIN32
  if ( signal( SIGPIPE, SIG_IGN) == SIG_ERR)
  {
    cerr << "Couldn't install signal handler for SIGPIPE" << endl;
    exit(-1);
  }
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

  // Initialize network
  initNetwork();

	try
	{ 
		InfoLog(<< argv[0] << " starting");

		std::string peerName     = argc >= 2 ? argv[1] : "b2bua@nohost";
		std::string nodeName     = argc >= 3 ? argv[2] : "b2bua_cnode@nohost";
		std::string secretcookie = argc >= 4 ? argv[3] : "secretcookie";

		messaging::MessageTransport messageTransport;
		msgTransport = &messageTransport;

		cnode::Properties props(peerName, nodeName, secretcookie);
		cnode::Client nodeClient(props, messageTransport);
		nodeClient.run();

		gateway::CallFactory callFactory;
		siptricks::UaThread thread(callFactory, messageTransport);
		uaThread = &thread;
		thread.run();

		// use functor to handle messages.
		gateway::Callback callback(nodeClient, thread.userAgent());
		messageTransport.processMessages(callback);
	}
#ifdef USE_SSL
   catch (BaseSecurity::Exception& e)
   {
      WarningLog (<< "Couldn't set up security object: " << e);
      exit(-1);
   }
#endif
   catch (BaseException& e)
   {
      ErrLog (<< "Caught: " << e);
      exit(-1);
   }
   catch( ... )
   {
      ErrLog( << "Caught non-resip exception" );
      exit(-1);
   }

   return 0;
}

/* ====================================================================

 Copyright (c) 2011, SIP Spectrum, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are 
 met:

 1. Redistributions of source code must retain the above copyright 
    notice, this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution. 

 3. Neither the name of SIP Spectrum nor the names of its contributors 
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

