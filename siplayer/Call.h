#pragma once

#include <map>
#include <string>

namespace resip
{
	class SipMessage;
}

namespace siptricks
{
	class UserAgent;
	namespace fsm
	{
		class AbstractStateMachine;
	}

	class Call
	{
	public:
		typedef std::map<std::string, Call*> Map;

		Call(UserAgent& ua, fsm::AbstractStateMachine& fsm);
		~Call();

		fsm::AbstractStateMachine& getStateMachine();
		virtual void terminateCall() = 0;

		UserAgent& getUserAgent();

		resip::SipMessage& originalSipMessage();
		const resip::SipMessage& originalSipMessage() const;

		std::string& id();
		const std::string& callId() const;

		void save();
		void remove();

	private:
		UserAgent& mUserAgent;
		fsm::AbstractStateMachine* mStateMachine;
		resip::SipMessage* mOriginalSipMessage;
		std::string mCallId;
	};

}
