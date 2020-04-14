#pragma once

#include "siplayer/Call.h"
#include <resip/dum/DialogSet.hxx>

namespace gateway
{

	class Call : public siptricks::Call
	{
	public:
		Call(siptricks::UserAgent& ua, siptricks::fsm::AbstractStateMachine& fsm);
		virtual ~Call();

		resip::AppDialogSetHandle& serverDialogSetHandle();
		const resip::AppDialogSetHandle& serverDialogSetHandle() const;
		resip::AppDialogSetHandle& clientDialogSetHandle();
		const resip::AppDialogSetHandle& clientDialogSetHandle() const;

		virtual void terminateCall();

	private:
		resip::AppDialogSetHandle mServerDialogSetHandle;
		resip::AppDialogSetHandle mClientDialogSetHandle;
	};

}