#pragma once

#include <memory>
#include <resip/dum/ServerInviteSession.hxx>

using namespace resip;

namespace siptricks
{
	namespace fsm
	{

		class EventPayload
		{
		public:
			EventPayload();
			virtual ~EventPayload();

			typedef std::auto_ptr<EventPayload> Ptr;

			static EventPayload::Ptr createPayload(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg);
		};

		class NewServerSessionPayload : public EventPayload
		{
		public:
			NewServerSessionPayload(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg);
			virtual ~NewServerSessionPayload();

			ServerInviteSessionHandle mServerInviteSessionHandle;
			InviteSession::OfferAnswerType mOfferAnswerType;
			const SipMessage& mSipMessage;
		};

	}
}
