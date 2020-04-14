#include <assert.h>

#include "StateFactory.h"
#include "siplayer/StateChanger.h"
#include "StateMachine.h"
#include "siplayer/State.h"


namespace softphone
{
	namespace fsm
	{

		StateMachine::StateMachine(siptricks::fsm::StateFactory& factory, siptricks::fsm::StateChanger& changer, siptricks::fsm::Actions& actions) :
			mActions(actions),
			mStateContext(factory, changer, actions),
			mCurrentState(NULL),
			mFactory(factory)
		{
			changer.setStateMachine(*this);
			factory.createStates(mStateContext);
			mCurrentState = &factory.getState(siptricks::fsm::StateFactory::StateInitial);
		}

		StateMachine::~StateMachine() 
		{
		}

		void StateMachine::transitionTo(siptricks::fsm::State &newState)
		{
			mCurrentState = &newState;
		}

		/// called when an initial INVITE or the intial response to an outoing invite  
		void StateMachine::onClientNewSession(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onClientNewSession(evtPayload);
		}
		void StateMachine::onServerNewSession(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onServerNewSession(evtPayload);
		}

		/// Received a failure response from UAS
		void StateMachine::onFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onFailure(evtPayload);
		}

		/// called when an in-dialog provisional response is received that contains a body
		void StateMachine::onEarlyMedia(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onEarlyMedia(evtPayload);
		}

		/// called when dialog enters the Early state - typically after getting 18x
		void StateMachine::onProvisional(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onProvisional(evtPayload);
		}

		/// called when a dialog initiated as a UAC enters the connected state
		void StateMachine::onClientConnected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onClientConnected(evtPayload);
		}

		/// called when a dialog initiated as a UAS enters the connected state
		void StateMachine::onServerConnected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onServerConnected(evtPayload);
		}

		void StateMachine::onTerminated(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onTerminated(evtPayload);
		}

		/// called when a fork that was created through a 1xx never receives a 2xx
		/// because another fork answered and this fork was canceled by a proxy. 
		void StateMachine::onForkDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onForkDestroyed(evtPayload);
		}

		/// called when a 3xx with valid targets is encountered in an early dialog     
		/// This is different then getting a 3xx in onTerminated, as another
		/// request will be attempted, so the DialogSet will not be destroyed.
		/// Basically an onTermintated that conveys more information.
		/// checking for 3xx respones in onTerminated will not work as there may
		/// be no valid targets.
		void StateMachine::onRedirected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onRedirected(evtPayload);
		}

		/// called when an answer is received - has nothing to do with user
		/// answering the call 
		void StateMachine::onAnswer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onAnswer(evtPayload);
		}

		/// called when an offer is received - must send an answer soon after this
		void StateMachine::onOffer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onOffer(evtPayload);
		}

		/// called when an Invite w/out offer is sent, or any other context which
		/// requires an offer from the user
		void StateMachine::onOfferRequired(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onOfferRequired(evtPayload);
		}

		/// called if an offer in a UPDATE or re-INVITE was rejected - not real
		/// useful. A SipMessage is provided if one is available
		void StateMachine::onOfferRejected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onOfferRejected(evtPayload);
		}

		/// called when INFO message is received 
		/// the application must call acceptNIT() or rejectNIT()
		/// once it is ready for another message.
		void StateMachine::onInfo(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onInfo(evtPayload);
		}

		/// called when response to INFO message is received 
		void StateMachine::onInfoSuccess(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onInfoSuccess(evtPayload);
		}
		void StateMachine::onInfoFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onInfoFailure(evtPayload);
		}

		/// called when MESSAGE message is received 
		void StateMachine::onMessage(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onMessage(evtPayload);
		}

		/// called when response to MESSAGE message is received 
		void StateMachine::onMessageSuccess(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onMessageSuccess(evtPayload);
		}
		void StateMachine::onMessageFailure(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onMessageFailure(evtPayload);
		}

		/// called when an REFER message is received.  The refer is accepted or
		/// rejected using the server subscription. If the offer is accepted,
		/// DialogUsageManager::makeInviteSessionFromRefer can be used to create an
		/// InviteSession that will send notify messages using the ServerSubscription
		void StateMachine::onRefer(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onRefer(evtPayload);
		}

		void StateMachine::onReferNoSub(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onReferNoSub(evtPayload);
		}

		/// called when an REFER message receives a failure response 
		void StateMachine::onReferRejected(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onReferRejected(evtPayload);
		}

		/// called when an REFER message receives an accepted response 
		void StateMachine::onReferAccepted(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onReferAccepted(evtPayload);
		}

		void StateMachine::onCallDialogSetDestroyed(siptricks::fsm::EventPayload::Ptr evtPayload)
		{
			assert(mCurrentState);
			mCurrentState->onCallDialogSetDestroyed(evtPayload);
		}

		siptricks::fsm::State::Name StateMachine::report() const
		{
			return mCurrentState->getName();
		}
	}
}
