#pragma once

namespace resip
{
	class DialogUsageManager;
}

namespace siptricks
{
	class Call;
	class UserAgent;
	class CallDialogSet;
	
	class CallFactory
	{
	public:
		CallFactory() { }
		virtual ~CallFactory() { }

		virtual siptricks::Call* createCall(UserAgent& userAgent) = 0;
		virtual siptricks::CallDialogSet* createCallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager) = 0;
	};

}
