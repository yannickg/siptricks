#include "MyConversationManager.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>
#include <recon/ReconSubsystem.hxx>
#include <recon/MediaResourceParticipant.hxx>
#include <recon/Conversation.hxx>

#include "ConversationManagerSettings.h"
#include "ConversationManagerDelegate.h"

// Test Prompts for cache testing
#include "playback_prompt.h"
#include "record_prompt.h"

#define RESIPROCATE_SUBSYSTEM ReconSubsystem::RECON

using namespace resip;
using namespace recon;

MyConversationManager::MyConversationManager(const ConversationManagerSettings& settings, ConversationManagerDelegate& delegate)
      : ConversationManager(settings.mLocalAudioEnabled, settings.mMediaInterfaceMode, settings.mDefaultSampleRate, settings.mMaxSampleRate),
        mDelegate(delegate)
{ 
}

void 
MyConversationManager::notifyMediaEvent(ConversationHandle conversationHandle, int mediaConnectionId, MediaEvent::MediaEventType eventType)
{
   // default implementation
   ConversationManager::notifyMediaEvent(conversationHandle, mediaConnectionId, eventType);
}

void
MyConversationManager::onConversationDestroyed(ConversationHandle convHandle)
{
   mDelegate.onConversationDestroyed(convHandle);
}

void
MyConversationManager::onParticipantDestroyed(ParticipantHandle partHandle)
{
   mDelegate.onParticipantDestroyed(partHandle);
}

void
MyConversationManager::onDtmfEvent(ParticipantHandle partHandle, int dtmf, int duration, bool up)
{
   mDelegate.onDtmfEvent(partHandle, dtmf, duration, up);
}

void
MyConversationManager::onIncomingParticipant(ParticipantHandle partHandle, const SipMessage& msg, bool autoAnswer, ConversationProfile& conversationProfile)
{
   mDelegate.onIncomingParticipant(partHandle, msg, autoAnswer, conversationProfile);
}

void
MyConversationManager::onRequestOutgoingParticipant(ParticipantHandle partHandle, const SipMessage& msg, ConversationProfile& conversationProfile)
{
   mDelegate.onRequestOutgoingParticipant(partHandle, msg, conversationProfile);
}
 
void
MyConversationManager::onParticipantTerminated(ParticipantHandle partHandle, unsigned int statusCode)
{
   mDelegate.onParticipantTerminated(partHandle, statusCode);
}
 
void
MyConversationManager::onParticipantProceeding(ParticipantHandle partHandle, const SipMessage& msg)
{
   mDelegate.onParticipantProceeding(partHandle, msg);
}

void
MyConversationManager::onRelatedConversation(ConversationHandle relatedConvHandle, ParticipantHandle relatedPartHandle, 
                                             ConversationHandle origConvHandle, ParticipantHandle origPartHandle)
{
   mDelegate.onRelatedConversation(relatedConvHandle, relatedPartHandle, origConvHandle, origPartHandle);
}

void
MyConversationManager::onParticipantAlerting(ParticipantHandle partHandle, const SipMessage& msg)
{
   mDelegate.onParticipantAlerting(partHandle, msg);
}
    
void
MyConversationManager::onParticipantConnected(ParticipantHandle partHandle, const SipMessage& msg)
{
   mDelegate.onParticipantConnected(partHandle, msg);
}

void
MyConversationManager::onParticipantRedirectSuccess(ParticipantHandle partHandle)
{
   mDelegate.onParticipantRedirectSuccess(partHandle);
}

void
MyConversationManager::onParticipantRedirectFailure(ParticipantHandle partHandle, unsigned int statusCode)
{
   mDelegate.onParticipantRedirectFailure(partHandle, statusCode);
}

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

