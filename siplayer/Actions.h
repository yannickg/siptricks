#pragma once

#include "EventPayload.h"

namespace siptricks
{
	namespace fsm
	{

		class Actions
		{
		public:
			Actions();
			virtual ~Actions();

			virtual void provisional(int code, EventPayload::Ptr evtPayload) = 0;
		};

	}
}
