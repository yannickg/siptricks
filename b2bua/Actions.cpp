#include "Actions.h"
#include "siplayer/EventPayload.h"

namespace gateway
{
	namespace fsm
	{

		Actions::Actions()
		{
		}


		Actions::~Actions()
		{
		}

		void Actions::provisional(int code, siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			siptricks::fsm::NewServerSessionPayload* payload = dynamic_cast<siptricks::fsm::NewServerSessionPayload*>(evtPayload.get());
			payload->mServerInviteSessionHandle->provisional(code);
		}
	}
}
