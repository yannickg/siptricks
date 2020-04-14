#pragma once

#include "siplayer/CallFactory.h"

namespace gateway
{

	class CallFactory : public siptricks::CallFactory
	{
	public:
		CallFactory();
		virtual ~CallFactory();

		virtual siptricks::Call* createCall(siptricks::UserAgent& userAgent);
		virtual siptricks::CallDialogSet* createCallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager);
	};

}
