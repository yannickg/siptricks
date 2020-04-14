/*
 * Callback.cpp
 *
 *  Created on: 2014-05-10
 *      Author: anniel1
 */

#include "Callback.h"
#include "helper/Message.h"
#include "MessageTranslator.h"
#include "cnode/Client.h"
#include "cnode/Message.h"
#include "siplayer/UserAgent.h"

namespace gateway
{

Callback::Callback(cnode::Client& connection, siptricks::UserAgent& ua) :
		mNodeClient(connection),
		mUserAgent(ua)
{
}

void Callback::handleSupervisorMessage(messaging::Message& message)
{
	switch(message.type())
	{
	case messaging::Message::StartUserAgent:
		if (!mUserAgent.mRunning)
		{
			// TODO: set master/user profiles
			mUserAgent.initialize();
			// TODO: use given ports info
			mUserAgent.addTransports();
			mUserAgent.start();
		}
		break;
	case messaging::Message::StopUserAgent:
		mUserAgent.stop();
		break;
	default:
		// send a message to sip stack
		resip::DumCommand* command = utilities::MessageTranslator::fromSupervisorMessage(message);
		mUserAgent.send(*command);
		break;
	}
}

void Callback::handleWireMessage(messaging::Message& message)
{
	cnode::Message::Ptr msg(utilities::MessageTranslator::fromStackMessage(message));
	mNodeClient.send(msg);
}

void Callback::operator()(messaging::Message& message)
{
	// Shutdown messages always come from supervisor
	// so we schedule shutdown user agent immediately
	// and bail out.
	if (message.shuttingDown())
	{
		mUserAgent.shutdown();
	}
	else
	{
		// Check origin of message
		//    -   SIP -> Erlang
		//    -Erlang -> SIP
		switch (message.source())
		{
		case messaging::Message::Supervisor:
		{
			handleSupervisorMessage(message);
			break;
		}
		case messaging::Message::Wire:
		{
			handleWireMessage(message);
			break;
		}
		default:
			break;
		}
	}
}

} /* namespace gateway */
