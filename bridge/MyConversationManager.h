#ifndef MYCONVERSATIONMANAGER_H
#define MYCONVERSATIONMANAGER_H

#include <os/OsIntTypes.h>

#if defined(HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <rutil/Data.hxx>
#include <recon/ConversationManager.hxx>


using namespace resip;

class ConversationManagerSettings;
class ConversationManagerDelegate;

namespace recon
{

class MyConversationManager : public ConversationManager
{
public:

   MyConversationManager(const ConversationManagerSettings& settings, ConversationManagerDelegate& delegate);
   virtual ~MyConversationManager() {};

   virtual void notifyMediaEvent(ConversationHandle conversationHandle, int mediaConnectionId, MediaEvent::MediaEventType eventType);

   virtual void onConversationDestroyed(ConversationHandle convHandle);
   virtual void onParticipantDestroyed(ParticipantHandle partHandle);
   virtual void onDtmfEvent(ParticipantHandle partHandle, int dtmf, int duration, bool up);
   virtual void onIncomingParticipant(ParticipantHandle partHandle, const SipMessage& msg, bool autoAnswer, ConversationProfile& conversationProfile);
   virtual void onRequestOutgoingParticipant(ParticipantHandle partHandle, const SipMessage& msg, ConversationProfile& conversationProfile);
   virtual void onParticipantTerminated(ParticipantHandle partHandle, unsigned int statusCode);
   virtual void onParticipantProceeding(ParticipantHandle partHandle, const SipMessage& msg);
   virtual void onRelatedConversation(ConversationHandle relatedConvHandle, ParticipantHandle relatedPartHandle, 
                                      ConversationHandle origConvHandle, ParticipantHandle origPartHandle);
   virtual void onParticipantAlerting(ParticipantHandle partHandle, const SipMessage& msg);
   virtual void onParticipantConnected(ParticipantHandle partHandle, const SipMessage& msg);
   virtual void onParticipantRedirectSuccess(ParticipantHandle partHandle);
   virtual void onParticipantRedirectFailure(ParticipantHandle partHandle, unsigned int statusCode);

protected:
   ConversationManagerDelegate& mDelegate;
};

}

#endif

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

