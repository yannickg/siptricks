#pragma once


namespace siptricks
{
	namespace fsm
	{
		class State;
		class AbstractStateMachine;

		class StateChanger
		{
		public:
			StateChanger();
			virtual ~StateChanger();

			// Change to the given new State.
			virtual void transitionTo(State& newState);
			virtual void setStateMachine(AbstractStateMachine& stateMachine);

		private:
			AbstractStateMachine* mStateMachine;
		};

	}
}