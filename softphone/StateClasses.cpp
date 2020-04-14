#include "StateClasses.h"
#include "siplayer/StateContext.h"
#include "siplayer/StateChanger.h"
#include "StateFactory.h"
#include "Actions.h"

namespace softphone
{
	namespace fsm
	{
		//////////////////////////////////////////////////////////////////////
		siptricks::fsm::State::Name Initial::name = "Initial";
		//////////////////////////////////////////////////////////////////////

		Initial::Initial(siptricks::fsm::StateContext &c) :
			AbstractState(c, name)
		{
		}


		Initial::~Initial()
		{
		}

		void Initial::onServerNewSession(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			mContext.mActions.provisional(180, evtPayload);
			// TODO send signal here.
			transitionTo(siptricks::fsm::StateFactory::StateRinging);
		}

		//////////////////////////////////////////////////////////////////////
		siptricks::fsm::State::Name Ringing::name = "Ringing";
		//////////////////////////////////////////////////////////////////////

		Ringing::Ringing(siptricks::fsm::StateContext &c) :
			AbstractState(c, name)
		{
		}


		Ringing::~Ringing()
		{
		}


	}
}