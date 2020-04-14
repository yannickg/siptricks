#pragma once

#include <resip/dum/MasterProfile.hxx>
#include <resip/dum/DumShutdownHandler.hxx>
#include <resip/dum/ClientRegistration.hxx>
#include <semaphore.h>

#include "InviteSessionHandler.h"
#include "Call.h"

namespace resip
{
	class EventStackThread;
	class EventThreadInterruptor;
	class DialogUsageManager;
	class ThreadIf;
}

namespace messaging
{
	class MessageTransport;
}

namespace siptricks
{
	class CallFactory;
	class CallDialogSet;

	namespace fsm
	{
		class AbstractStateMachine;
	}

	class ShutdownHandler : public resip::DumShutdownHandler
	{
	public:
		ShutdownHandler(bool& dumShutdown);
		virtual void onDumCanBeDeleted();
	private:
		bool& mDumShutdown;
	};

	class UserAgent
	{
	public:
		UserAgent(siptricks::CallFactory& callFactory,
				messaging::MessageTransport& messageTransport,
				resip::ThreadIf& thread);
		virtual ~UserAgent();

		void initialize();
		void start();

		void stop();
		void shutdown();

		void cleanup();

		bool process(int timeoutMs);

		resip::DialogUsageManager& getDialogUsageManager() { return *mDum; }

		siptricks::CallDialogSet* createCallDialogSet();

		messaging::MessageTransport& getMessageTransport();

		void saveCall(const std::string& callId, Call& call);
		void send(resip::DumCommand& command);

		void addTransports();

		resip::ThreadIf& thread();
		const resip::ThreadIf& thread() const;

		bool sleepThread();
		void wakeUpThread();

		bool mRunning;

	protected:
		void addTransport(TransportType type, int port);

		siptricks::CallFactory& mCallFactory;

		SharedPtr<MasterProfile> mProfile;
		Security* mSecurity;
		FdPollGrp* mPollGrp;
		EventThreadInterruptor* mInterruptor;
		SipStack* mStack;
		EventStackThread* mStackThread;
		DialogUsageManager* mDum;
		volatile bool mDumShutdownRequested;
		bool mShuttingdown;
		bool mDumShutdown;
		ClientRegistrationHandle mRegHandle;
		ShutdownHandler mShutdownHandler;

		friend class Call;
		siptricks::Call::Map mCallMap;

		siptricks::InviteSessionHandler mInviteSessionHandler;
		messaging::MessageTransport& mMessageTransport;
		resip::ThreadIf& mThread;

	    sem_t mSemaphore;
	};

}
