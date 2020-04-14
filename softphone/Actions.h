#pragma once

#include "siplayer/Actions.h"

namespace softphone
{
	namespace fsm
	{

		class Actions : public siptricks::fsm::Actions
		{
		public:
			Actions();
			virtual ~Actions();

			virtual void provisional(int code, siptricks::fsm::EventPayload::Ptr evtPayload);
		};

	}
}

