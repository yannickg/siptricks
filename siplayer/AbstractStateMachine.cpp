#include "AbstractStateMachine.h"
#include <assert.h>


namespace siptricks
{
	namespace fsm
	{

		AbstractStateMachine::AbstractStateMachine()
		{
		}

		AbstractStateMachine::~AbstractStateMachine()
		{
		}

		void AbstractStateMachine::transitionTo(State &newState)
		{
			assert(false);
		}

		void AbstractStateMachine::onEntry()
		{
		}

		void AbstractStateMachine::onExit()
		{
		}

		/// called when an initial INVITE or the intial response to an outoing invite  
		void AbstractStateMachine::onClientNewSession(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractStateMachine::onServerNewSession(EventPayload::Ptr evtPayload)
		{
		}

		/// Received a failure response from UAS
		void AbstractStateMachine::onFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an in-dialog provisional response is received that contains a body
		void AbstractStateMachine::onEarlyMedia(EventPayload::Ptr evtPayload)
		{
		}

		/// called when dialog enters the Early state - typically after getting 18x
		void AbstractStateMachine::onProvisional(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAC enters the connected state
		void AbstractStateMachine::onClientConnected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAS enters the connected state
		void AbstractStateMachine::onServerConnected(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractStateMachine::onTerminated(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a fork that was created through a 1xx never receives a 2xx
		/// because another fork answered and this fork was canceled by a proxy. 
		void AbstractStateMachine::onForkDestroyed(EventPayload::Ptr evtPayload)
		{
		}

		/// called when a 3xx with valid targets is encountered in an early dialog     
		/// This is different then getting a 3xx in onTerminated, as another
		/// request will be attempted, so the DialogSet will not be destroyed.
		/// Basically an onTermintated that conveys more information.
		/// checking for 3xx respones in onTerminated will not work as there may
		/// be no valid targets.
		void AbstractStateMachine::onRedirected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an answer is received - has nothing to do with user
		/// answering the call 
		void AbstractStateMachine::onAnswer(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an offer is received - must send an answer soon after this
		void AbstractStateMachine::onOffer(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an Invite w/out offer is sent, or any other context which
		/// requires an offer from the user
		void AbstractStateMachine::onOfferRequired(EventPayload::Ptr evtPayload)
		{
		}

		/// called if an offer in a UPDATE or re-INVITE was rejected - not real
		/// useful. A SipMessage is provided if one is available
		void AbstractStateMachine::onOfferRejected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when INFO message is received 
		/// the application must call acceptNIT() or rejectNIT()
		/// once it is ready for another message.
		void AbstractStateMachine::onInfo(EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to INFO message is received 
		void AbstractStateMachine::onInfoSuccess(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractStateMachine::onInfoFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when MESSAGE message is received 
		void AbstractStateMachine::onMessage(EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to MESSAGE message is received 
		void AbstractStateMachine::onMessageSuccess(EventPayload::Ptr evtPayload)
		{
		}
		void AbstractStateMachine::onMessageFailure(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message is received.  The refer is accepted or
		/// rejected using the server subscription. If the offer is accepted,
		/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
		/// InviteSession that will send notify messages using the ServerSubscription
		void AbstractStateMachine::onRefer(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractStateMachine::onReferNoSub(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives a failure response 
		void AbstractStateMachine::onReferRejected(EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives an accepted response 
		void AbstractStateMachine::onReferAccepted(EventPayload::Ptr evtPayload)
		{
		}

		void AbstractStateMachine::onCallDialogSetDestroyed(EventPayload::Ptr evtPayload)
		{
		}

		State::Name AbstractStateMachine::report() const
		{
			return "";
		}
	}
}