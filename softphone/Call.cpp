#include "Call.h"
#include <resip/dum/AppDialogSet.hxx>


namespace softphone
{

	Call::Call(siptricks::UserAgent& ua, siptricks::fsm::AbstractStateMachine& fsm) :
		siptricks::Call(ua, fsm)
	{
	}


	Call::~Call()
	{
	}

	resip::AppDialogSetHandle& Call::dialogSetHandle()
	{
		return mDialogSetHandle;
	}

	const resip::AppDialogSetHandle& Call::dialogSetHandle() const
	{
		return mDialogSetHandle;
	}

	void Call::terminateCall()
	{
		mDialogSetHandle->end();
	}

}