#pragma once

namespace siptricks
{
	namespace fsm
	{

		class StateFactory;
		class StateChanger;
		class Actions;

		//------------------------------------------------------------------------------
		//
		class StateContext
			//
			/// @brief The context that a State needs.
			///
			//------------------------------------------------------------------------------
		{
		public:

			StateContext(StateFactory &f, StateChanger &c, Actions &a)
				:
				mFactory(f),
				mChanger(c),
				mActions(a)
			{};

			StateFactory &mFactory;
			StateChanger &mChanger;
			Actions &mActions;
		};

	}
}