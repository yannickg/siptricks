#pragma once
#include "siplayer/CallDialogSet.h"

namespace gateway
{

	class CallDialogSet : public siptricks::CallDialogSet
	{
	public:
		CallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager);
		virtual ~CallDialogSet();
	};

}