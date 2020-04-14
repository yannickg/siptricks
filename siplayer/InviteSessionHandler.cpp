#include <resip/dum/ServerInviteSession.hxx>

#include "InviteSessionHandler.h"
#include "CallDialogSet.h"
#include "EventPayload.h"
#include "AbstractStateMachine.h"
#include "State.h"
#include "Call.h"


namespace siptricks
{

	InviteSessionHandler::InviteSessionHandler()
	{
	}


	InviteSessionHandler::~InviteSessionHandler()
	{
	}

	/// called when an initial INVITE or the intial response to an outoing invite  
	void InviteSessionHandler::onNewSession(ClientInviteSessionHandle, InviteSession::OfferAnswerType oat, const SipMessage& msg)
	{
	}
	void InviteSessionHandler::onNewSession(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg)
	{
		fsm::EventPayload::Ptr ptr = fsm::EventPayload::createPayload(h, oat, msg);
		dynamic_cast<CallDialogSet*>(h->getAppDialogSet().get())->getCall().getStateMachine().onServerNewSession(ptr);
	}

	/// Received a failure response from UAS
	void InviteSessionHandler::onFailure(ClientInviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when an in-dialog provisional response is received that contains a body
	void InviteSessionHandler::onEarlyMedia(ClientInviteSessionHandle, const SipMessage&, const SdpContents&)
	{
	}

	/// called when dialog enters the Early state - typically after getting 18x
	void InviteSessionHandler::onProvisional(ClientInviteSessionHandle, const SipMessage&)
	{
	}

	/// called when a dialog initiated as a UAC enters the connected state
	void InviteSessionHandler::onConnected(ClientInviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when a dialog initiated as a UAS enters the connected state
	void InviteSessionHandler::onConnected(InviteSessionHandle, const SipMessage& msg)
	{
	}

	void InviteSessionHandler::onTerminated(InviteSessionHandle, InviteSessionHandler::TerminatedReason reason, const SipMessage* related /* = 0 */)
	{
	}

	/// called when a fork that was created through a 1xx never receives a 2xx
	/// because another fork answered and this fork was canceled by a proxy. 
	void InviteSessionHandler::onForkDestroyed(ClientInviteSessionHandle)
	{
	}

	/// called when a 3xx with valid targets is encountered in an early dialog     
	/// This is different then getting a 3xx in onTerminated, as another
	/// request will be attempted, so the DialogSet will not be destroyed.
	/// Basically an onTermintated that conveys more information.
	/// checking for 3xx respones in onTerminated will not work as there may
	/// be no valid targets.
	void InviteSessionHandler::onRedirected(ClientInviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when an answer is received - has nothing to do with user
	/// answering the call 
	void InviteSessionHandler::onAnswer(InviteSessionHandle, const SipMessage& msg, const SdpContents&)
	{
	}

	/// called when an offer is received - must send an answer soon after this
	void InviteSessionHandler::onOffer(InviteSessionHandle, const SipMessage& msg, const SdpContents&)
	{
	}

	/// called when an Invite w/out offer is sent, or any other context which
	/// requires an offer from the user
	void InviteSessionHandler::onOfferRequired(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called if an offer in a UPDATE or re-INVITE was rejected - not real
	/// useful. A SipMessage is provided if one is available
	void InviteSessionHandler::onOfferRejected(InviteSessionHandle, const SipMessage* msg)
	{
	}

	/// called when INFO message is received 
	/// the application must call acceptNIT() or rejectNIT()
	/// once it is ready for another message.
	void InviteSessionHandler::onInfo(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when response to INFO message is received 
	void InviteSessionHandler::onInfoSuccess(InviteSessionHandle, const SipMessage& msg)
	{
	}
	void InviteSessionHandler::onInfoFailure(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when MESSAGE message is received 
	void InviteSessionHandler::onMessage(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when response to MESSAGE message is received 
	void InviteSessionHandler::onMessageSuccess(InviteSessionHandle, const SipMessage& msg)
	{
	}
	void InviteSessionHandler::onMessageFailure(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when an REFER message is received.  The refer is accepted or
	/// rejected using the server subscription. If the offer is accepted,
	/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
	/// InviteSession that will send notify messages using the ServerSubscription
	void InviteSessionHandler::onRefer(InviteSessionHandle, ServerSubscriptionHandle, const SipMessage& msg)
	{
	}

	void InviteSessionHandler::onReferNoSub(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when an REFER message receives a failure response 
	void InviteSessionHandler::onReferRejected(InviteSessionHandle, const SipMessage& msg)
	{
	}

	/// called when an REFER message receives an accepted response 
	void InviteSessionHandler::onReferAccepted(InviteSessionHandle, ClientSubscriptionHandle, const SipMessage& msg)
	{
	}
}