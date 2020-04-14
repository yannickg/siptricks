#ifndef CONVERSATION_H 
#define CONVERSATION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <recon/HandleTypes.hxx>
#include <map>

namespace recon
{
class ConversationManager;
}

namespace bridge
{

class Conversation
{
public:
   typedef std::map<resip::Data, bridge::Conversation> Map;

   Conversation(const bridge::Conversation& rhs);
   Conversation(recon::ConversationHandle convHandle, bool recordConversation);
   Conversation();

   class Participant
   {
   public:
      typedef std::map<resip::Data, bridge::Conversation::Participant> Map;

      Participant();
      Participant(const resip::Data& address);
      Participant(const bridge::Conversation::Participant& rhs);
      Participant(const recon::ParticipantHandle& partHandle);
      Participant(const resip::Data& address, recon::ParticipantHandle partHandle);
      
      const resip::Data& uuid() const;
      resip::Data& uuid();

      const resip::Data& address() const;
      resip::Data& address();

      const recon::ParticipantHandle& handle() const;
      recon::ParticipantHandle& handle();

      Participant& operator= (const Participant& rhsParticipant);

      void setConversation(bridge::Conversation* conversation);

      bool uninvited() const;
      void removeFromConversation();
      bool isConversationOwner() const;

   protected:
      resip::Data mUuid;
      resip::Data mAddress;
      recon::ParticipantHandle mHandle;
      bridge::Conversation* mConversation;
   };

   const resip::Data& uuid() const;
   resip::Data& uuid();

   const recon::ConversationHandle& handle() const;
   recon::ConversationHandle& handle();

   const bool& recordConversation() const;
   bool& recordConversation();

   const Conversation::Participant* mediaParticipant() const;
   Conversation::Participant* mediaParticipant();

   const Conversation::Participant* recordingParticipant() const;
   Conversation::Participant* recordingParticipant();

   Conversation& operator= (const Conversation& rhs);

   void setConversationOwner(const resip::Data& address, const recon::ParticipantHandle& handle);
   void addNextParticipant(const resip::Data& address);
   void setMediaParticipant(const recon::ParticipantHandle& handle);
   void setRecordingParticipant(const recon::ParticipantHandle& handle);

   Conversation::Participant* getNextParticipantToInvite();

   bool hasThisParticipant(const recon::ParticipantHandle& handle, Conversation::Participant*& part);
   bool participantIsConversationOwner(const recon::ParticipantHandle& handle);

   void removeParticipant(const bridge::Conversation::Participant& part);
   bool canDeleteConversation() const;

   void deleteAllParticipants(recon::ConversationManager& convManager);

protected:
   resip::Data mUuid;
   recon::ConversationHandle mConvHandle;
   Conversation::Participant* mConversationOwner;
   Conversation::Participant* mMediaParticipant;
   Conversation::Participant* mRecordingParticipant;
   Conversation::Participant::Map mParticipants;
   bool mRecordConversation;
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
