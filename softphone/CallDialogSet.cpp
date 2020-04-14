#include "CallDialogSet.h"
#include "Call.h"


namespace softphone
{

	CallDialogSet::CallDialogSet(siptricks::Call* call, resip::DialogUsageManager& dialogUsageManager) :
		siptricks::CallDialogSet(call, dialogUsageManager)
	{
		dynamic_cast<Call&>(mCall).dialogSetHandle() = getHandle();
	}


	CallDialogSet::~CallDialogSet()
	{
	}

}
