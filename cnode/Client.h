/*
 * Client.h
 *
 *  Created on: 2014-05-03
 *      Author: anniel1
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include "Message.h"

namespace messaging
{
	class MessageTransport;
}

namespace cnode
{

class Connection;
class Properties;

class Client
{
public:
	Client(const cnode::Properties& props,
			messaging::MessageTransport& messageTransport);
	virtual ~Client();

	void run();
	void execute();

	void send(cnode::Message::Ptr msg);

private:
	Connection* mConnection;
    std::string mPeerName;
    std::string mNodeName;
    std::string mSecretCookie;
	bool mRunning;
	messaging::MessageTransport& mMessageTransport;
};

} /* namespace cnode */

#endif /* CONNECTION_H_ */
