/*
 * Client.cpp
 *
 *  Created on: 2014-05-03
 *      Author: anniel1
 */

#include "Client.h"
#include "Connection.h"
#include "Properties.h"
#include "helper/MessageTransport.h"
#include "Shutdown.h"
#include <iostream>

namespace cnode {

Client::Client(const cnode::Properties& props,
		messaging::MessageTransport& messageTransport) :
	mConnection(NULL),
    mPeerName(props.mPeerName),
    mNodeName(props.mNodeName),
    mSecretCookie(props.mSecretCookie),
	mRunning(false),
	mMessageTransport(messageTransport)
{
}

Client::~Client()
{
	if (mConnection)
	{
		delete mConnection;
	}
}

void Client::send(cnode::Message::Ptr msg)
{
	mConnection->send(msg->erlangMessage());
}

void *thread_proc(void* x)
{
	Client* self = reinterpret_cast<Client*>(x);
	self->execute();

	pthread_exit(NULL);
}

void Client::run()
{
    pthread_t t1;
    int res = pthread_create(&t1, NULL, thread_proc, this);
    if (res)
    {
    	// throw an exception
    }
}

void Client::execute()
{
	mConnection = new Connection(mNodeName, mSecretCookie);
	if (mConnection->connect(mPeerName))
	{
		mRunning = true;

		while (mRunning)
		{
			// TODO: pass a msg to receive
			messaging::Message* message = NULL;
			if (mConnection->receive(&message))
			{
				// TODO: post the msg return by receive.
				mMessageTransport.postMessage(*message);
			}
			else
			{
				Shutdown* message = new Shutdown();
				mMessageTransport.postMessage(*message);
				break;
			}
		}

		fprintf(stderr, "quitting!\n");
		mRunning = false;
	}
	else
	{
		// TODO: throw an exception
		throw;
	}
}

} /* namespace cnode */
