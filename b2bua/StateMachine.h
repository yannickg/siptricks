#pragma once

#include "siplayer/AbstractStateMachine.h"
#include "siplayer/StateContext.h"
#include "siplayer/State.h"

namespace gateway
{
	namespace fsm
	{
		class StateMachine : public siptricks::fsm::AbstractStateMachine
		{
		public:
			StateMachine();
			virtual ~StateMachine();

		private:
			// for lint.
			StateMachine(const StateMachine &);
			StateMachine & operator= (const StateMachine &);

		public:
			virtual void transitionTo(siptricks::fsm::State &newState);

			// Get the current state, in an abstract way.
			virtual siptricks::fsm::State::Name report() const;

			/// called when an initial INVITE or the intial response to an outoing invite  
			virtual void onClientNewSession(siptricks::fsm::EventPayload::Ptr evtPayload);
			virtual void onServerNewSession(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// Received a failure response from UAS
			virtual void onFailure(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an in-dialog provisional response is received that contains a body
			virtual void onEarlyMedia(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when dialog enters the Early state - typically after getting 18x
			virtual void onProvisional(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when a dialog initiated as a UAC enters the connected state
			virtual void onClientConnected(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when a dialog initiated as a UAS enters the connected state
			virtual void onServerConnected(siptricks::fsm::EventPayload::Ptr evtPayload);

			virtual void onTerminated(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when a fork that was created through a 1xx never receives a 2xx
			/// because another fork answered and this fork was canceled by a proxy. 
			virtual void onForkDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when a 3xx with valid targets is encountered in an early dialog     
			/// This is different then getting a 3xx in onTerminated, as another
			/// request will be attempted, so the DialogSet will not be destroyed.
			/// Basically an onTermintated that conveys more information.
			/// checking for 3xx respones in onTerminated will not work as there may
			/// be no valid targets.
			virtual void onRedirected(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an answer is received - has nothing to do with user
			/// answering the call 
			virtual void onAnswer(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an offer is received - must send an answer soon after this
			virtual void onOffer(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an Invite w/out offer is sent, or any other context which
			/// requires an offer from the user
			virtual void onOfferRequired(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called if an offer in a UPDATE or re-INVITE was rejected - not real
			/// useful. A SipMessage is provided if one is available
			virtual void onOfferRejected(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when INFO message is received 
			/// the application must call acceptNIT() or rejectNIT()
			/// once it is ready for another message.
			virtual void onInfo(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when response to INFO message is received 
			virtual void onInfoSuccess(siptricks::fsm::EventPayload::Ptr evtPayload);
			virtual void onInfoFailure(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when MESSAGE message is received 
			virtual void onMessage(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when response to MESSAGE message is received 
			virtual void onMessageSuccess(siptricks::fsm::EventPayload::Ptr evtPayload);
			virtual void onMessageFailure(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an REFER message is received.  The refer is accepted or
			/// rejected using the server subscription. If the offer is accepted,
			/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
			/// InviteSession that will send notify messages using the ServerSubscription
			virtual void onRefer(siptricks::fsm::EventPayload::Ptr evtPayload);

			virtual void onReferNoSub(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an REFER message receives a failure response 
			virtual void onReferRejected(siptricks::fsm::EventPayload::Ptr evtPayload);

			/// called when an REFER message receives an accepted response 
			virtual void onReferAccepted(siptricks::fsm::EventPayload::Ptr evtPayload);

			virtual void onCallDialogSetDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload);
		};

	}
}
