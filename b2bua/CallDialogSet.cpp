#include "CallDialogSet.h"
#include "Call.h"


namespace gateway
{

	CallDialogSet::CallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager) :
		siptricks::CallDialogSet(call, dialogUsageManager)
	{
		// the given dialogSetHandle (first one) is always server. The second
		// one will be client.
		dynamic_cast<Call&>(mCall).serverDialogSetHandle() = getHandle();
	}


	CallDialogSet::~CallDialogSet()
	{
	}

}
