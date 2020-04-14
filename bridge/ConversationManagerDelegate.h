#ifndef CONVERSATIONMANAGERDELEGATE_H 
#define CONVERSATIONMANAGERDELEGATE_H 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <resip/stack/SipMessage.hxx>
#include <recon/HandleTypes.hxx>
#include <recon/ConversationProfile.hxx>

using namespace resip;
using namespace recon;

class ConversationManagerDelegate
{
public:
   ConversationManagerDelegate();

   virtual void onConversationDestroyed(ConversationHandle convHandle) = 0;
   virtual void onParticipantDestroyed(ParticipantHandle partHandle) = 0;
   virtual void onDtmfEvent(ParticipantHandle partHandle, int dtmf, int duration, bool up) = 0;
   virtual void onIncomingParticipant(ParticipantHandle partHandle, const SipMessage& msg, bool autoAnswer, ConversationProfile& conversationProfile) = 0;
   virtual void onRequestOutgoingParticipant(ParticipantHandle partHandle, const SipMessage& msg, ConversationProfile& conversationProfile) = 0;
   virtual void onParticipantTerminated(ParticipantHandle partHandle, unsigned int statusCode) = 0;
   virtual void onParticipantProceeding(ParticipantHandle partHandle, const SipMessage& msg) = 0;
   virtual void onRelatedConversation(ConversationHandle relatedConvHandle, ParticipantHandle relatedPartHandle, 
                                      ConversationHandle origConvHandle, ParticipantHandle origPartHandle) = 0;
   virtual void onParticipantAlerting(ParticipantHandle partHandle, const SipMessage& msg) = 0;
   virtual void onParticipantConnected(ParticipantHandle partHandle, const SipMessage& msg) = 0;
   virtual void onParticipantRedirectSuccess(ParticipantHandle partHandle) = 0;
   virtual void onParticipantRedirectFailure(ParticipantHandle partHandle, unsigned int statusCode) = 0;
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
