#include "Call.h"
#include "UserAgent.h"
#include "AbstractStateMachine.h"

namespace siptricks
{

	Call::Call(UserAgent& ua, fsm::AbstractStateMachine& fsm) :
		mUserAgent(ua),
		mStateMachine(&fsm),
		mOriginalSipMessage(new resip::SipMessage())
	{
	}

	Call::~Call()
	{
		delete mOriginalSipMessage;
		delete mStateMachine;
	}

	fsm::AbstractStateMachine& Call::getStateMachine()
	{
		return *mStateMachine;
	}

	UserAgent& Call::getUserAgent()
	{
		return mUserAgent;
	}

	resip::SipMessage& Call::originalSipMessage()
	{
		return *mOriginalSipMessage;
	}

	const resip::SipMessage& Call::originalSipMessage() const
	{		
		return *mOriginalSipMessage;
	}

	std::string& Call::id()
	{
		return mCallId;
	}

	const std::string& Call::callId() const
	{
		return mCallId;
	}

	void Call::save()
	{
		mUserAgent.saveCall(mCallId, *this);
	}

	void Call::remove()
	{
	}
}
