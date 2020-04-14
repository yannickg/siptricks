#pragma once

#include "rutil/ThreadIf.hxx"

namespace messaging
{
	class MessageTransport;
}

namespace siptricks
{
	class CallFactory;
	class UserAgent;

	class UaThread : public resip::ThreadIf
	{
	public:
		UaThread(siptricks::CallFactory& callFactory,
				messaging::MessageTransport& messageTransport);
		~UaThread();

		virtual void thread();

		UserAgent& userAgent();

	private:
		UserAgent* mUserAgent;
	};

}
