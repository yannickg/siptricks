#include "AbstractState.h"
#include "StateContext.h"
#include "StateChanger.h"

namespace siptricks
{
	namespace fsm
	{
		std::string AbstractState::defaultName = "default";

		AbstractState::AbstractState(StateContext &c, const State::Name& n) :
			mContext(c),
			mName(n)
		{
		}

		AbstractState::~AbstractState()
		{
		}

		void AbstractState::transitionTo(StateFactory::FactoryState factoryState)
		{
			mContext.mChanger.transitionTo(mContext.mFactory.getState(factoryState));
		}

		void AbstractState::onEntry()
		{			
		}

		void AbstractState::onExit()
		{			
		}

		/// called when an initial INVITE or the intial response to an outoing invite  
		void AbstractState::onClientNewSession(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractState::onServerNewSession(EventPayload::Ptr evtPayload)
		{
		}

		/// Received a failure response from UAS
		void AbstractState::onFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an in-dialog provisional response is received that contains a body
		void AbstractState::onEarlyMedia(EventPayload::Ptr evtPayload)
		{
		}

		/// called when dialog enters the Early state - typically after getting 18x
		void AbstractState::onProvisional(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAC enters the connected state
		void AbstractState::onClientConnected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAS enters the connected state
		void AbstractState::onServerConnected(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractState::onTerminated(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a fork that was created through a 1xx never receives a 2xx
		/// because another fork answered and this fork was canceled by a proxy. 
		void AbstractState::onForkDestroyed(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a 3xx with valid targets is encountered in an early dialog     
		/// This is different then getting a 3xx in onTerminated, as another
		/// request will be attempted, so the DialogSet will not be destroyed.
		/// Basically an onTermintated that conveys more information.
		/// checking for 3xx respones in onTerminated will not work as there may
		/// be no valid targets.
		void AbstractState::onRedirected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an answer is received - has nothing to do with user
		/// answering the call 
		void AbstractState::onAnswer(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an offer is received - must send an answer soon after this
		void AbstractState::onOffer(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an Invite w/out offer is sent, or any other context which
		/// requires an offer from the user
		void AbstractState::onOfferRequired(EventPayload::Ptr evtPayload)
		{
		}

		/// called if an offer in a UPDATE or re-INVITE was rejected - not real
		/// useful. A SipMessage is provided if one is available
		void AbstractState::onOfferRejected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when INFO message is received 
		/// the application must call acceptNIT() or rejectNIT()
		/// once it is ready for another message.
		void AbstractState::onInfo(EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to INFO message is received 
		void AbstractState::onInfoSuccess(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractState::onInfoFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when MESSAGE message is received 
		void AbstractState::onMessage(EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to MESSAGE message is received 
		void AbstractState::onMessageSuccess(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractState::onMessageFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message is received.  The refer is accepted or
		/// rejected using the server subscription. If the offer is accepted,
		/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
		/// InviteSession that will send notify messages using the ServerSubscription
		void AbstractState::onRefer(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractState::onReferNoSub(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives a failure response 
		void AbstractState::onReferRejected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives an accepted response 
		void AbstractState::onReferAccepted(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractState::onCallDialogSetDestroyed(EventPayload::Ptr evtPayload)
		{
		}
	}
}