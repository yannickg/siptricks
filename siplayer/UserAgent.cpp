#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>
#include <resip/dum/AppDialogSetFactory.hxx>
#include <resip/stack/EventStackThread.hxx>
#include <resip/dum/DialogUsageManager.hxx>

#include <utility>
#include <algorithm>

#include "UserAgent.h"
#include "CallDialogSet.h"
#include "StateFactory.h"
#include "StateChanger.h"
#include "CallFactory.h"
#include "Call.h"

#include "helper/MessageTransport.h"

#define RESIPROCATE_SUBSYSTEM Subsystem::APP

namespace siptricks
{
	class CallDialogSetFactory : public AppDialogSetFactory
	{ 
	public:
		CallDialogSetFactory(UserAgent& ua) : mUserAgent(ua) {}
		resip::AppDialogSet* createAppDialogSet(DialogUsageManager& dum, const SipMessage& msg)
		{
			switch (msg.method())
			{
			case INVITE:
				return mUserAgent.createCallDialogSet();
				break;
			default:
				return AppDialogSetFactory::createAppDialogSet(dum, msg);
				break;
			}
		}
	private:
		UserAgent& mUserAgent;
	};

	ShutdownHandler::ShutdownHandler(bool& dumShutdown) :
		mDumShutdown(dumShutdown)
	{		
	}

	void ShutdownHandler::onDumCanBeDeleted()
	{
		mDumShutdown = true;
	}

	UserAgent::UserAgent(siptricks::CallFactory& callFactory,
			messaging::MessageTransport& messageTransport,
			resip::ThreadIf& thread) :
				mProfile(new MasterProfile),
				mCallFactory(callFactory),
				mMessageTransport(messageTransport),
				mShutdownHandler(mDumShutdown),
				mThread(thread)
	{
		const char* logType = "cout";
		const char* logLevel = "INFO";
		const char* appName = "softphone";
		Log::initialize(logType, logLevel, appName);

	    sem_init(&mSemaphore, 1, 0);

	    initialize();
	}

	UserAgent::~UserAgent()
	{
		cleanup();
	}

	void UserAgent::initialize()
	{
		mRunning = false;
		mPollGrp = FdPollGrp::create();
		mInterruptor = new EventThreadInterruptor(*mPollGrp);
		mStack = new SipStack;
		mStackThread = new EventStackThread(*mStack, *mInterruptor, *mPollGrp);
		mDum = new DialogUsageManager(*mStack);
		mDumShutdownRequested = false;
		mShuttingdown = false;
		mDumShutdown = false;

		// profile
		mDum->setInviteSessionHandler(&mInviteSessionHandler);
		std::auto_ptr<AppDialogSetFactory> dialogSetFactory(new CallDialogSetFactory(*this));
		mDum->setAppDialogSetFactory(dialogSetFactory);

		mDum->setMasterProfile(mProfile);
	}

	void UserAgent::cleanup()
	{
		mStack->shutdownAndJoinThreads();
		mStackThread->shutdown();
		mStackThread->join();

		delete mDum;
		delete mStack;
		delete mStackThread;
		delete mInterruptor;
		delete mPollGrp;
	}

	void UserAgent::start()
	{
		mStack->run();
		mStackThread->run();
		wakeUpThread();
		mRunning = true;
		mDumShutdown = false;
	}

	struct terminateCall
	{
		void operator()(std::pair<std::string const, siptricks::Call*>& map_assoc_)
		{
			map_assoc_.second->terminateCall();
		}
	};

	bool UserAgent::process(int timeoutMs)
	{
		if (!mDumShutdown)
		{
			if (mDumShutdownRequested)
			{
				// unregister
				if (mRegHandle.isValid())
				{
					mRegHandle->end();
				}

				// End all calls - copy list in case delete/unregister of call is immediate
				siptricks::Call::Map callMap = mCallMap;
				std::for_each(
						callMap.begin(),
						callMap.end(),
						terminateCall()
						);

				mDum->shutdown(&mShutdownHandler);
				mDumShutdownRequested = false;
			}

			mDum->process(timeoutMs);
			return true;
		}

		return false;
	}

	void UserAgent::stop()
	{
		assert(mDum);
		mDumShutdownRequested = true; // Set flag so that shutdown operations can be run in dum process thread
		mRunning = false;
	}

	void UserAgent::shutdown()
	{
		assert(mDum);
		mDumShutdownRequested = true; // Set flag so that shutdown operations can be run in dum process thread
		mShuttingdown = true;         // This flag stays on during the shutdown process where as mDumShutdownRequested will get toggled back to false
		wakeUpThread();
		mRunning = false;
	}

	void UserAgent::addTransports()
	{
		addTransport(UDP, 5060);
		addTransport(TCP, 5060);
	}

	void UserAgent::addTransport(TransportType type, int port)
	{
		if (port == 0) return;  // Transport disabled

		try
		{
			mStack->addTransport(type, port, V4, StunEnabled);
		}
		catch (BaseException& e)
		{
			InfoLog(<< "Caught: " << e);
			WarningLog(<< "Failed to add " << Tuple::toData(type) << " transport on " << port);
		}
	}

	siptricks::CallDialogSet* UserAgent::createCallDialogSet()
	{
		// These 2 instructions could be merged into just calling
		// createCallDialogSet which would be responsible to 
		// also create the call.
		siptricks::Call* call = mCallFactory.createCall(*this);
		return mCallFactory.createCallDialogSet(call, *mDum);
	}

	messaging::MessageTransport& UserAgent::getMessageTransport()
	{
		return mMessageTransport;
	}

	void UserAgent::saveCall(const std::string& callId, Call& call)
	{
		mCallMap[callId] = &call;
	}

	void UserAgent::send(resip::DumCommand& command)
	{
		std::auto_ptr<resip::ApplicationMessage> message(&command);
		mStack->post(message, 1, mDum);
	}

	resip::ThreadIf& UserAgent::thread()
	{
		return mThread;
	}

	const resip::ThreadIf& UserAgent::thread() const
	{
		return mThread;
	}

	bool UserAgent::sleepThread()
	{
	    sem_wait(&mSemaphore);
	    return !mShuttingdown;
	}

	void UserAgent::wakeUpThread()
	{
	    sem_post(&mSemaphore);
	}
}
