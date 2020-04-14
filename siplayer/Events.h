#pragma once

#include "EventPayload.h"

namespace siptricks
{
	namespace fsm
	{
		class Events
		{
		public:
			Events();
			virtual ~Events();

			virtual void onEntry() = 0;
			virtual void onExit() = 0;

			/// called when an initial INVITE or the intial response to an outoing invite  
			virtual void onClientNewSession(EventPayload::Ptr evtPayload) = 0;
			virtual void onServerNewSession(EventPayload::Ptr evtPayload) = 0;

			/// Received a failure response from UAS
			virtual void onFailure(EventPayload::Ptr evtPayload) = 0;

			/// called when an in-dialog provisional response is received that contains a body
			virtual void onEarlyMedia(EventPayload::Ptr evtPayload) = 0;

			/// called when dialog enters the Early state - typically after getting 18x
			virtual void onProvisional(EventPayload::Ptr evtPayload) = 0;

			/// called when a dialog initiated as a UAC enters the connected state
			virtual void onClientConnected(EventPayload::Ptr evtPayload) = 0;

			/// called when a dialog initiated as a UAS enters the connected state
			virtual void onServerConnected(EventPayload::Ptr evtPayload) = 0;

			virtual void onTerminated(EventPayload::Ptr evtPayload) = 0;

			/// called when a fork that was created through a 1xx never receives a 2xx
			/// because another fork answered and this fork was canceled by a proxy. 
			virtual void onForkDestroyed(EventPayload::Ptr evtPayload) = 0;

			/// called when a 3xx with valid targets is encountered in an early dialog     
			/// This is different then getting a 3xx in onTerminated, as another
			/// request will be attempted, so the DialogSet will not be destroyed.
			/// Basically an onTermintated that conveys more information.
			/// checking for 3xx respones in onTerminated will not work as there may
			/// be no valid targets.
			virtual void onRedirected(EventPayload::Ptr evtPayload) = 0;

			/// called when an answer is received - has nothing to do with user
			/// answering the call 
			virtual void onAnswer(EventPayload::Ptr evtPayload) = 0;

			/// called when an offer is received - must send an answer soon after this
			virtual void onOffer(EventPayload::Ptr evtPayload) = 0;

			/// called when an Invite w/out offer is sent, or any other context which
			/// requires an offer from the user
			virtual void onOfferRequired(EventPayload::Ptr evtPayload) = 0;

			/// called if an offer in a UPDATE or re-INVITE was rejected - not real
			/// useful. A SipMessage is provided if one is available
			virtual void onOfferRejected(EventPayload::Ptr evtPayload) = 0;

			/// called when INFO message is received 
			/// the application must call acceptNIT() or rejectNIT()
			/// once it is ready for another message.
			virtual void onInfo(EventPayload::Ptr evtPayload) = 0;

			/// called when response to INFO message is received 
			virtual void onInfoSuccess(EventPayload::Ptr evtPayload) = 0;
			virtual void onInfoFailure(EventPayload::Ptr evtPayload) = 0;

			/// called when MESSAGE message is received 
			virtual void onMessage(EventPayload::Ptr evtPayload) = 0;

			/// called when response to MESSAGE message is received 
			virtual void onMessageSuccess(EventPayload::Ptr evtPayload) = 0;
			virtual void onMessageFailure(EventPayload::Ptr evtPayload) = 0;

			/// called when an REFER message is received.  The refer is accepted or
			/// rejected using the server subscription. If the offer is accepted,
			/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
			/// InviteSession that will send notify messages using the ServerSubscription
			virtual void onRefer(EventPayload::Ptr evtPayload) = 0;

			virtual void onReferNoSub(EventPayload::Ptr evtPayload) = 0;

			/// called when an REFER message receives a failure response 
			virtual void onReferRejected(EventPayload::Ptr evtPayload) = 0;

			/// called when an REFER message receives an accepted response 
			virtual void onReferAccepted(EventPayload::Ptr evtPayload) = 0;

			virtual void onCallDialogSetDestroyed(EventPayload::Ptr evtPayload) = 0;
		};

	}
}