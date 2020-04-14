#ifndef RECONSERVER_H 
#define RECONSERVER_H 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Server.h"
#include "ConversationManagerDelegate.h"
#include "Conversation.h"
#include "AddressTranslator.h"

class MyUserAgent;

namespace recon
{
class ConversationManager;
class ReConServerConfig;
}

class reConServer : public Server, public ConversationManagerDelegate
{
public:
   reConServer();
   virtual ~reConServer();

   void startup(recon::ReConServerConfig& reConServerConfig);
   void process(int timeoutMs);
   void shutdown();

   // Server interface definition
   virtual resip::Data createConversation(const resip::Data& caller, const resip::Data& callee, bool recordConversation = true);
   virtual void dropVoiceMail(const resip::Data& conversationId);

   virtual bool translateAddress(const resip::Data& address, resip::Data& translation, bool failIfNoRule = false);

   // ConversationManagerDelegate interface definition
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
   void prepareConversation(recon::ConversationHandle convHandle, const resip::Data& caller, const resip::Data& callee, bool recordConversation);
   bridge::Conversation* conversationFromParticipanHandle(recon::ParticipantHandle partHandle, bridge::Conversation::Participant*& part);

   recon::ConversationManager* mConversationManager;
   MyUserAgent* mUserAgent;

   bridge::Conversation::Map mConversations;

   clicktocall::AddressTranslator mAddressTranslator;
};

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
