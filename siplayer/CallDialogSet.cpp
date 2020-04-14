#include "CallDialogSet.h"
#include "UserAgent.h"
#include "Call.h"

namespace siptricks
{

	CallDialogSet::CallDialogSet(Call* call, resip::DialogUsageManager& dialogUsageManager) :
		AppDialogSet(dialogUsageManager),
		mCall(*call)
	{
	}

	CallDialogSet::~CallDialogSet()
	{
	}

	Call& CallDialogSet::getCall()
	{
		return mCall;
	}

	const Call& CallDialogSet::getCall() const
	{
		return mCall;
	}
}