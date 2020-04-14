#include "EventPayload.h"


namespace siptricks
{
	namespace fsm
	{

		EventPayload::EventPayload()
		{
		}


		EventPayload::~EventPayload()
		{
		}

		EventPayload::Ptr EventPayload::createPayload(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg)
		{
			EventPayload::Ptr ptr(new NewServerSessionPayload(h, oat, msg));
			return ptr;
		}

		NewServerSessionPayload::NewServerSessionPayload(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg) :
			mServerInviteSessionHandle(h),
			mOfferAnswerType(oat),
			mSipMessage(msg)
		{
		}

		NewServerSessionPayload::~NewServerSessionPayload()
		{
		}
	}
}