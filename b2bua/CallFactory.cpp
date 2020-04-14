#include "CallFactory.h"

#include "siplayer/StateChanger.h"
#include "Actions.h"
#include "StateMachine.h"
#include "Call.h"
#include "CallDialogSet.h"

namespace gateway
{

	CallFactory::CallFactory()
	{

	}

	CallFactory::~CallFactory()
	{
	}

	siptricks::Call* CallFactory::createCall(siptricks::UserAgent& userAgent)
	{
		siptricks::fsm::AbstractStateMachine* fsm = new gateway::fsm::StateMachine();
		return new gateway::Call(userAgent, *fsm);
	}

	siptricks::CallDialogSet* CallFactory::createCallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager)
	{
		return new CallDialogSet(call, dialogUsageManager);
	}
}
