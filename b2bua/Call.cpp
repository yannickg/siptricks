#include "Call.h"
#include <resip/dum/AppDialogSet.hxx>


namespace gateway
{

	Call::Call(siptricks::UserAgent& ua, siptricks::fsm::AbstractStateMachine& fsm) :
		siptricks::Call(ua, fsm)
	{
	}

	Call::~Call()
	{
	}

	resip::AppDialogSetHandle& Call::serverDialogSetHandle()
	{
		return mServerDialogSetHandle;
	}

	const resip::AppDialogSetHandle& Call::serverDialogSetHandle() const
	{
		return mServerDialogSetHandle;
	}

	resip::AppDialogSetHandle& Call::clientDialogSetHandle()
	{
		return mClientDialogSetHandle;
	}

	const resip::AppDialogSetHandle& Call::clientDialogSetHandle() const
	{
		return mClientDialogSetHandle;
	}

	void Call::terminateCall()
	{
		if (mServerDialogSetHandle.isValid())
		{
			mServerDialogSetHandle->end();
		}
	}

}
