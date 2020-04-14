#include "CallFactory.h"

#include "StateFactory.h"
#include "siplayer/StateChanger.h"
#include "Actions.h"
#include "StateMachine.h"
#include "Call.h"
#include "CallDialogSet.h"

namespace softphone
{

	CallFactory::CallFactory() :
		mStateFactory(new softphone::fsm::StateFactory()),
		mStateChanger(new siptricks::fsm::StateChanger()),
		mActions(new softphone::fsm::Actions())
	{

	}

	CallFactory::~CallFactory()
	{
		delete mStateFactory;
		delete mStateChanger;
		delete mActions;
	}

	siptricks::Call* CallFactory::createCall(siptricks::UserAgent& userAgent)
	{
		siptricks::fsm::AbstractStateMachine* fsm = 
			new softphone::fsm::StateMachine(*mStateFactory, *mStateChanger, *mActions);
		return new softphone::Call(userAgent, *fsm);
	}

	siptricks::CallDialogSet* CallFactory::createCallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager)
	{
		return new CallDialogSet(call, dialogUsageManager);
	}
}
