#pragma once

#include <resip/dum/AppDialogSet.hxx>


namespace siptricks
{
	class UserAgent;
	class Call;

	class CallDialogSet : public resip::AppDialogSet
	{
	public:
		CallDialogSet(Call* call, resip::DialogUsageManager& dialogUsageManager);
		virtual ~CallDialogSet();

		Call& getCall();
		const Call& getCall() const;

	protected:
		Call& mCall;
	};

}