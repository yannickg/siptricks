#include "StateFactory.h"
#include "StateClasses.h"

namespace softphone
{
	namespace fsm
	{

		StateFactory::StateFactory() :
			mDefaultState(NULL)
		{
		}

		StateFactory::~StateFactory()
		{
		}

		void StateFactory::createStates(siptricks::fsm::StateContext& context)
		{
			mDefaultState = new siptricks::fsm::AbstractState(context);
			mStateMap[siptricks::fsm::StateFactory::StateInitial] = new Initial(context);
			mStateMap[siptricks::fsm::StateFactory::StateRinging] = new Ringing(context);
		}

		siptricks::fsm::State& StateFactory::getState(siptricks::fsm::StateFactory::FactoryState state)
		{
			siptricks::fsm::State::Map::iterator iter;
			if ((iter = mStateMap.find(state)) != mStateMap.end())
			{
				return *iter->second;
			}

			assert(false);
			return *mDefaultState;
		}
	}
}
