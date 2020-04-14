#pragma once

namespace siptricks
{
	namespace fsm
	{
		class State;
		class Actions;
		class StateContext;

		//------------------------------------------------------------------------------
		//
		class StateFactory
		//
		/// @brief Interface describing something which supplies State objects.
		///
		//------------------------------------------------------------------------------
		{
		public:
			enum FactoryState
			{
				StateInitial,
				StateRinging,
				StateBlah
			};

			virtual ~StateFactory() {};

			virtual State& getState(StateFactory::FactoryState state) = 0;
			virtual void createStates(StateContext& context) = 0;
		};

	}
}
