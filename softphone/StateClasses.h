#pragma once

#include "siplayer/AbstractState.h"

namespace softphone
{
	namespace fsm
	{

		class Initial : public siptricks::fsm::AbstractState
		{
		public:
			Initial(siptricks::fsm::StateContext &c);
			virtual ~Initial();

			static siptricks::fsm::State::Name name;

			virtual void onServerNewSession(siptricks::fsm::EventPayload::Ptr evtPayload);
		};

		class Ringing : public siptricks::fsm::AbstractState
		{
		public:
			Ringing(siptricks::fsm::StateContext &c);
			virtual ~Ringing();

			static siptricks::fsm::State::Name name;
		};
	}
}