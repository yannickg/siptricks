#include "StateChanger.h"
#include "AbstractStateMachine.h"
#include <assert.h>

namespace siptricks
{
	namespace fsm
	{

		StateChanger::StateChanger() :
			mStateMachine(NULL)
		{
		}

		StateChanger::~StateChanger()
		{
		}

		void StateChanger::transitionTo(State &newState)
		{
			assert(mStateMachine);
			mStateMachine->onExit();
			mStateMachine->transitionTo(newState);
			mStateMachine->onEntry();
		}

		void StateChanger::setStateMachine(AbstractStateMachine& stateMachine)
		{
			mStateMachine = &stateMachine;
		}
	}
}