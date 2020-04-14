#pragma once

#include "resip/dum/InviteSessionHandler.hxx"

using namespace resip;

namespace siptricks
{

	class InviteSessionHandler : public resip::InviteSessionHandler
	{
	public:
		InviteSessionHandler();
		virtual ~InviteSessionHandler();

		/// called when an initial INVITE or the intial response to an outoing invite  
		virtual void onNewSession(ClientInviteSessionHandle, InviteSession::OfferAnswerType oat, const SipMessage& msg);
		virtual void onNewSession(ServerInviteSessionHandle, InviteSession::OfferAnswerType oat, const SipMessage& msg);

		/// Received a failure response from UAS
		virtual void onFailure(ClientInviteSessionHandle, const SipMessage& msg);

		/// called when an in-dialog provisional response is received that contains a body
		virtual void onEarlyMedia(ClientInviteSessionHandle, const SipMessage&, const SdpContents&);

		/// called when dialog enters the Early state - typically after getting 18x
		virtual void onProvisional(ClientInviteSessionHandle, const SipMessage&);

		/// called when a dialog initiated as a UAC enters the connected state
		virtual void onConnected(ClientInviteSessionHandle, const SipMessage& msg);

		/// called when a dialog initiated as a UAS enters the connected state
		virtual void onConnected(InviteSessionHandle, const SipMessage& msg);

		virtual void onTerminated(InviteSessionHandle, InviteSessionHandler::TerminatedReason reason, const SipMessage* related = 0);

		/// called when a fork that was created through a 1xx never receives a 2xx
		/// because another fork answered and this fork was canceled by a proxy. 
		virtual void onForkDestroyed(ClientInviteSessionHandle);

		/// called when a 3xx with valid targets is encountered in an early dialog     
		/// This is different then getting a 3xx in onTerminated, as another
		/// request will be attempted, so the DialogSet will not be destroyed.
		/// Basically an onTermintated that conveys more information.
		/// checking for 3xx respones in onTerminated will not work as there may
		/// be no valid targets.
		virtual void onRedirected(ClientInviteSessionHandle, const SipMessage& msg);

		/// called when an answer is received - has nothing to do with user
		/// answering the call 
		virtual void onAnswer(InviteSessionHandle, const SipMessage& msg, const SdpContents&);

		/// called when an offer is received - must send an answer soon after this
		virtual void onOffer(InviteSessionHandle, const SipMessage& msg, const SdpContents&);

		/// called when an Invite w/out offer is sent, or any other context which
		/// requires an offer from the user
		virtual void onOfferRequired(InviteSessionHandle, const SipMessage& msg);

		/// called if an offer in a UPDATE or re-INVITE was rejected - not real
		/// useful. A SipMessage is provided if one is available
		virtual void onOfferRejected(InviteSessionHandle, const SipMessage* msg);

		/// called when INFO message is received 
		/// the application must call acceptNIT() or rejectNIT()
		/// once it is ready for another message.
		virtual void onInfo(InviteSessionHandle, const SipMessage& msg);

		/// called when response to INFO message is received 
		virtual void onInfoSuccess(InviteSessionHandle, const SipMessage& msg);
		virtual void onInfoFailure(InviteSessionHandle, const SipMessage& msg);

		/// called when MESSAGE message is received 
		virtual void onMessage(InviteSessionHandle, const SipMessage& msg);

		/// called when response to MESSAGE message is received 
		virtual void onMessageSuccess(InviteSessionHandle, const SipMessage& msg);
		virtual void onMessageFailure(InviteSessionHandle, const SipMessage& msg);

		/// called when an REFER message is received.  The refer is accepted or
		/// rejected using the server subscription. If the offer is accepted,
		/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
		/// InviteSession that will send notify messages using the ServerSubscription
		virtual void onRefer(InviteSessionHandle, ServerSubscriptionHandle, const SipMessage& msg);

		virtual void onReferNoSub(InviteSessionHandle, const SipMessage& msg);

		/// called when an REFER message receives a failure response 
		virtual void onReferRejected(InviteSessionHandle, const SipMessage& msg);

		/// called when an REFER message receives an accepted response 
		virtual void onReferAccepted(InviteSessionHandle, ClientSubscriptionHandle, const SipMessage& msg);
	};

}