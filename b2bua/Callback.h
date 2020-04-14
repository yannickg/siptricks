/*
 * Callback.h
 *
 *  Created on: 2014-05-10
 *      Author: anniel1
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "helper/Callback.h"

namespace siptricks
{
	class UserAgent;
}

namespace cnode
{

class Client;

}

namespace gateway
{

/*
 * reference: http://www.tutok.sk/fastgl/callback.html
 */
class Callback: public messaging::Callback
{
public:
	Callback(cnode::Client& connection, siptricks::UserAgent& ua);
	virtual void operator()(messaging::Message& message);

private:
	void handleSupervisorMessage(messaging::Message& message);
	void handleWireMessage(messaging::Message& message);

    cnode::Client& mNodeClient;
    siptricks::UserAgent& mUserAgent;
};

} /* namespace gateway */

#endif /* CALLBACK_H_ */
