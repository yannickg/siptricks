#pragma once

#include "StateFactory.h"
#include "siplayer/AbstractState.h"
#include "siplayer/State.h"

namespace softphone
{
	namespace fsm
	{

		class StateFactory : public siptricks::fsm::StateFactory
		{
		public:
			StateFactory();
			virtual ~StateFactory();

			virtual siptricks::fsm::State& getState(siptricks::fsm::StateFactory::FactoryState state);
			virtual void createStates(siptricks::fsm::StateContext& context);

		private:
			siptricks::fsm::State::Map mStateMap;
			siptricks::fsm::AbstractState* mDefaultState;
		};

	}
}
