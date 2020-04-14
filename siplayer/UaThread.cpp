#include "UaThread.h"
#include "UserAgent.h"
#include "CallFactory.h"
#include <rutil/Logger.hxx>

#define RESIPROCATE_SUBSYSTEM Subsystem::TEST

namespace siptricks
{

	UaThread::UaThread(
			siptricks::CallFactory& callFactory,
			messaging::MessageTransport& messageTransport
			) :
		mUserAgent(new UserAgent(callFactory, messageTransport, *this))
	{
	}


	UaThread::~UaThread()
	{
	}

	void UaThread::thread()
	{
		InfoLog(<< "Starting worker thread");

		while (true)
		{
			if (!mUserAgent->sleepThread())
			{
				break;
			}

			bool shuttingDown = false;
			while (mUserAgent->process(1000))  // Look for exit key every 1 second
			{
				if (isShutdown() && !shuttingDown)
				{
					InfoLog(<< "Scheduling worker thread shutdown");
					shuttingDown = true;
					mUserAgent->stop();
				}
			}

			mUserAgent->cleanup();
		}
	}

	UserAgent& UaThread::userAgent()
	{
		return *mUserAgent;
	}
}
