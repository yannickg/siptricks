#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <os/OsIntTypes.h>

#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>
#include <recon/ReconSubsystem.hxx>
#include <recon/ConversationManager.hxx>

#include "Conversation.h"
#include "Util.h"

#define RESIPROCATE_SUBSYSTEM ReconSubsystem::RECON

namespace bridge
{

Conversation::Conversation(recon::ConversationHandle convHandle, bool recordConversation) :
   mUuid(getVersion4UuidUrn()), mConvHandle(convHandle), mConversationOwner(NULL), 
   mMediaParticipant(NULL), mRecordingParticipant(NULL), 
   mRecordConversation(recordConversation)
{
}

Conversation::Conversation(const bridge::Conversation& rhs) :
   mUuid(rhs.mUuid), mConvHandle(rhs.mConvHandle), mConversationOwner(rhs.mConversationOwner),
   mMediaParticipant(rhs.mMediaParticipant), mRecordingParticipant(rhs.mRecordingParticipant), 
   mRecordConversation(rhs.mRecordConversation)
{
   mParticipants.insert(rhs.mParticipants.begin(), rhs.mParticipants.end());
}

Conversation::Conversation() :
   mUuid(getVersion4UuidUrn()), mConvHandle(-1), mConversationOwner(NULL),
   mMediaParticipant(NULL), mRecordingParticipant(NULL), 
   mRecordConversation(false)
{
}

Conversation& Conversation::operator= (const Conversation& rhs)
{
   mUuid = rhs.mUuid;
   mConvHandle = rhs.mConvHandle;
   mConversationOwner = rhs.mConversationOwner;
   mMediaParticipant = rhs.mMediaParticipant;
   mRecordingParticipant = rhs.mRecordingParticipant;
   mParticipants.clear();
   mParticipants.insert(rhs.mParticipants.begin(), rhs.mParticipants.end());
   mRecordConversation = rhs.mRecordConversation;
}

const resip::Data& Conversation::uuid() const
{
   return mUuid;
}

resip::Data& Conversation::uuid()
{
   return mUuid;
}

const recon::ConversationHandle& Conversation::handle() const
{
   return mConvHandle;
}

recon::ConversationHandle& Conversation::handle()
{
   return mConvHandle;
}

Conversation::Participant::Participant() :
   mUuid(getVersion4UuidUrn()), mAddress(resip::Data::Empty), mHandle(-1),
   mConversation(NULL)
{
}

Conversation::Participant::Participant(const resip::Data& address) :
   mUuid(getVersion4UuidUrn()), mAddress(address), mHandle(-1),
   mConversation(NULL)
{
}

Conversation::Participant::Participant(const bridge::Conversation::Participant& rhs) :
   mUuid(rhs.mUuid), mAddress(rhs.mAddress), mHandle(rhs.mHandle),
   mConversation(rhs.mConversation)
{
}

Conversation::Participant::Participant(const recon::ParticipantHandle& partHandle) :
   mUuid(getVersion4UuidUrn()), mAddress(resip::Data::Empty), mHandle(partHandle),
   mConversation(NULL)
{
}

Conversation::Participant::Participant(const resip::Data& address, recon::ParticipantHandle partHandle) :
   mUuid(getVersion4UuidUrn()), mAddress(address), mHandle(partHandle),
   mConversation(NULL)
{
}

const resip::Data& Conversation::Participant::uuid() const
{
   return mUuid;
}

resip::Data& Conversation::Participant::uuid()
{
   return mUuid;
}

const resip::Data& Conversation::Participant::address() const
{
   return mAddress;
}

resip::Data& Conversation::Participant::address()
{
   return mAddress;
}

const recon::ParticipantHandle& Conversation::Participant::handle() const
{
   return mHandle;
}

recon::ParticipantHandle& Conversation::Participant::handle()
{
   return mHandle;
}

Conversation::Participant& Conversation::Participant::operator= (const Participant& rhsParticipant)
{
   mUuid = rhsParticipant.mUuid;
   mAddress = rhsParticipant.mAddress;
   mHandle = rhsParticipant.mHandle;
   mConversation = rhsParticipant.mConversation;
   return *this;
}

void Conversation::Participant::setConversation(bridge::Conversation* conversation)
{
   DebugLog( << "Conversation::Participant::setConversation()");

   mConversation = conversation;
}

bool Conversation::Participant::uninvited() const
{
   return (mHandle == -1);
}

void Conversation::Participant::removeFromConversation()
{
   DebugLog( << "Conversation::Participant::removeFromConversation()");

   if (mConversation)
   {
      mConversation->removeParticipant(*this);
      mConversation = NULL;
   }
}

bool Conversation::Participant::isConversationOwner() const
{
   DebugLog( << "Conversation::Participant::isConversationOwner");

   bool conversationOwner = false;
   if (mConversation && mConversation->mConversationOwner)
   {
      DebugLog( << "Participant " << mConversation->mConversationOwner->handle() << " is member of conversation " << mConversation->handle());
      conversationOwner = (mConversation->mConversationOwner->handle() == handle());
   }
   return conversationOwner;
}

const bool& Conversation::recordConversation() const
{
   return mRecordConversation;
}

bool& Conversation::recordConversation()
{
   return mRecordConversation;
}

const Conversation::Participant* Conversation::mediaParticipant() const
{
   return mMediaParticipant;
}

Conversation::Participant* Conversation::mediaParticipant()
{
   return mMediaParticipant;
}

const Conversation::Participant* Conversation::recordingParticipant() const
{
   return mRecordingParticipant;
}

Conversation::Participant* Conversation::recordingParticipant()
{
   return mRecordingParticipant;
}

void Conversation::setConversationOwner(const resip::Data& address, const recon::ParticipantHandle& handle)
{
   DebugLog( << "Conversation::setConversationOwner: " << handle);
   assert(this);

   Conversation::Participant part(address, handle);
   part.setConversation(this);
   mParticipants[part.uuid()] = part;
   Conversation::Participant::Map::iterator iter = mParticipants.find(part.uuid());
   mConversationOwner = iter != mParticipants.end()? &iter->second: NULL;
}

void Conversation::addNextParticipant(const resip::Data& address)
{
   DebugLog( << "Conversation::addNextParticipant: " << address);
   assert(this);

   Conversation::Participant part(address);
   part.setConversation(this);
   mParticipants[part.uuid()] = part;
}

void Conversation::setMediaParticipant(const recon::ParticipantHandle& handle)
{
   DebugLog( << "Conversation::setMediaParticipant: " << handle);
   assert(this);

   Conversation::Participant part(handle);
   part.setConversation(this);
   mParticipants[part.uuid()] = part;
   mMediaParticipant = &mParticipants[part.uuid()];
}

void Conversation::setRecordingParticipant(const recon::ParticipantHandle& handle)
{
   DebugLog( << "Conversation::setRecordingParticipant: " << handle);
   assert(this);

   Conversation::Participant part(handle);
   part.setConversation(this);
   mParticipants[part.uuid()] = part;
   mRecordingParticipant = &mParticipants[part.uuid()];
}

Conversation::Participant* Conversation::getNextParticipantToInvite()
{
   DebugLog( << "Conversation::getNextParticipantToInvite()");

   Conversation::Participant* part = NULL;
   Conversation::Participant::Map::iterator iter = mParticipants.begin();
   for (; iter != mParticipants.end(); ++iter)
   {
      if (iter->second.uninvited())
      {
         part = &iter->second;
         break;
      }
   }
   return part;
}

bool Conversation::hasThisParticipant(const recon::ParticipantHandle& handle, Conversation::Participant*& part)
{
   DebugLog( << "Conversation::hasThisParticipant: " << handle);

   bool ret = false;
   if (participantIsConversationOwner(handle))
   {
      part = mConversationOwner;
      DebugLog( << "Participant found (conversation owner):  " << mConversationOwner->handle());
      ret = true;
   }
   else
   {
      Conversation::Participant::Map::iterator iter = mParticipants.begin();
      for (; iter != mParticipants.end(); ++iter)
      {
         if (iter->second.handle() == handle)
         {
            part = &iter->second;
            DebugLog( << "Participant found: " << iter->second.handle());
            ret = true;
            break;
         }
      }
   }
   return ret;
}

bool Conversation::participantIsConversationOwner(const recon::ParticipantHandle& handle)
{
   DebugLog( << "Conversation::participantIsConversationOwner: " << handle);

   if (mConversationOwner && mConversationOwner->handle() == handle)
   {
      return true;
   }
   return false;
}

void Conversation::removeParticipant(const bridge::Conversation::Participant& part)
{
   DebugLog( << "Conversation::removeParticipant: " << part.handle());

   if (mConversationOwner && mConversationOwner->handle() == part.handle())
   {
      DebugLog( << "Participant was conversation owner");
      mConversationOwner = NULL;
   }
   else if (mMediaParticipant && mMediaParticipant->handle() == part.handle())
   {
      DebugLog( << "Participant was media");
      mMediaParticipant = NULL;
   }
   else if (mRecordingParticipant && mRecordingParticipant->handle() == part.handle())
   {
      DebugLog( << "Participant was recording");
      mRecordingParticipant = NULL;
   }
   else
   {
      DebugLog( << "Just a simple participant");
   }

   Conversation::Participant::Map::iterator iter = mParticipants.begin();
   for (; iter != mParticipants.end(); ++iter)
   {
      if (iter->second.handle() == part.handle())
      {
         DebugLog( << "Found participant - removing participant " << iter->second.handle() << " from list");
         mParticipants.erase(iter);
         break;
      }
   }
}

bool Conversation::canDeleteConversation() const
{
   DebugLog( << "Conversation::canDeleteConversation()");

   int minParticipant = 2;
   if (mMediaParticipant) minParticipant++;
   if (mRecordingParticipant) minParticipant++;
   DebugLog( << "Current number of participants: " << mParticipants.size() << " Minimum number of participants to delete: " << minParticipant);
   return (mParticipants.size() < minParticipant);
}

void Conversation::deleteAllParticipants(recon::ConversationManager& convManager)
{
   DebugLog( << "Conversation::deleteAllParticipants()");

   Conversation::Participant::Map::iterator iter = mParticipants.begin();
   while (iter != mParticipants.end())
   {
      DebugLog( << "Deleting participant: " << iter->second.handle());
      convManager.destroyParticipant(iter->second.handle());
      mParticipants.erase(iter);
      iter = mParticipants.begin();
   }
}

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
