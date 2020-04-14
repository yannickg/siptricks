#pragma once

#include "siplayer/Call.h"
#include <resip/dum/DialogSet.hxx>

namespace softphone
{

	class Call : public siptricks::Call
	{
	public:
		Call(siptricks::UserAgent& ua, siptricks::fsm::AbstractStateMachine& fsm);
		virtual ~Call();

		resip::AppDialogSetHandle& dialogSetHandle();
		const resip::AppDialogSetHandle& dialogSetHandle() const;

		virtual void terminateCall();

	private:
		resip::AppDialogSetHandle mDialogSetHandle;
	};

}