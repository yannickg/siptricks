#pragma once

#include "siplayer/CallFactory.h"

namespace siptricks
{
	namespace fsm
	{
		class StateFactory;
		class StateChanger;
		class Actions;
	}
}

namespace softphone
{

	class CallFactory : public siptricks::CallFactory
	{
	public:
		CallFactory();
		virtual ~CallFactory();

		virtual siptricks::Call* createCall(siptricks::UserAgent& userAgent);
		virtual siptricks::CallDialogSet* createCallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager);

	private:
		siptricks::fsm::StateFactory* mStateFactory;
		siptricks::fsm::StateChanger* mStateChanger;
		siptricks::fsm::Actions* mActions;
	};

}