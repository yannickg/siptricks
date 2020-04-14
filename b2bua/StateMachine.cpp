#include <assert.h>

#include "StateMachine.h"
#include "siplayer/StateChanger.h"
#include "siplayer/State.h"
#include "siplayer/CallDialogSet.h"
#include "helper/MessageTransport.h"
#include "siplayer/UserAgent.h"
#include "siplayer/Call.h"
#include "NewSession.h"

#include <resip/stack/SipMessage.hxx>


namespace
{
	siptricks::Call& getCall(ServerInviteSessionHandle& handle)
	{
		return dynamic_cast<siptricks::CallDialogSet*>(handle->getAppDialogSet().get())->getCall();
	}
}

namespace gateway
{
	namespace fsm
	{

		StateMachine::StateMachine()
		{
		}

		StateMachine::~StateMachine() 
		{
		}

		void StateMachine::transitionTo(siptricks::fsm::State &newState)
		{
		}

		/// called when an initial INVITE or the intial response to an outoing invite  
		void StateMachine::onClientNewSession(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}
		void StateMachine::onServerNewSession(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			// post message to message queue (and eventually to erlang node)
			siptricks::fsm::NewServerSessionPayload* payload = dynamic_cast<siptricks::fsm::NewServerSessionPayload*>(evtPayload.get());
			siptricks::Call& call = getCall(payload->mServerInviteSessionHandle);

			// need to save this call first
			call.originalSipMessage() = payload->mSipMessage;
			call.id() = payload->mSipMessage.header(resip::h_CallId).value().c_str();
			call.save();

			// CallId and SipMessage need to go into message
			NewSession* message(new NewSession(call.id(), call.originalSipMessage()));
			call.getUserAgent().getMessageTransport().postMessage(*message);
		}

		/// Received a failure response from UAS
		void StateMachine::onFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an in-dialog provisional response is received that contains a body
		void StateMachine::onEarlyMedia(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when dialog enters the Early state - typically after getting 18x
		void StateMachine::onProvisional(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAC enters the connected state
		void StateMachine::onClientConnected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when a dialog initiated as a UAS enters the connected state
		void StateMachine::onServerConnected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		void StateMachine::onTerminated(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when a fork that was created through a 1xx never receives a 2xx
		/// because another fork answered and this fork was canceled by a proxy. 
		void StateMachine::onForkDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when a 3xx with valid targets is encountered in an early dialog     
		/// This is different then getting a 3xx in onTerminated, as another
		/// request will be attempted, so the DialogSet will not be destroyed.
		/// Basically an onTermintated that conveys more information.
		/// checking for 3xx respones in onTerminated will not work as there may
		/// be no valid targets.
		void StateMachine::onRedirected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an answer is received - has nothing to do with user
		/// answering the call 
		void StateMachine::onAnswer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an offer is received - must send an answer soon after this
		void StateMachine::onOffer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an Invite w/out offer is sent, or any other context which
		/// requires an offer from the user
		void StateMachine::onOfferRequired(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called if an offer in a UPDATE or re-INVITE was rejected - not real
		/// useful. A SipMessage is provided if one is available
		void StateMachine::onOfferRejected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when INFO message is received 
		/// the application must call acceptNIT() or rejectNIT()
		/// once it is ready for another message.
		void StateMachine::onInfo(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to INFO message is received 
		void StateMachine::onInfoSuccess(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}
		void StateMachine::onInfoFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when MESSAGE message is received 
		void StateMachine::onMessage(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when response to MESSAGE message is received 
		void StateMachine::onMessageSuccess(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}
		void StateMachine::onMessageFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message is received.  The refer is accepted or
		/// rejected using the server subscription. If the offer is accepted,
		/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
		/// InviteSession that will send notify messages using the ServerSubscription
		void StateMachine::onRefer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		void StateMachine::onReferNoSub(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives a failure response 
		void StateMachine::onReferRejected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		/// called when an REFER message receives an accepted response 
		void StateMachine::onReferAccepted(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		void StateMachine::onCallDialogSetDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
		}

		siptricks::fsm::State::Name StateMachine::report() const
		{
			return std::string("string");
		}
	}
}
