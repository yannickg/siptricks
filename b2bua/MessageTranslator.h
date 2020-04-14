/*
 * MessageTranslator.h
 *
 *  Created on: 2014-05-11
 *      Author: anniel1
 */

#ifndef MESSAGETRANSLATOR_H_
#define MESSAGETRANSLATOR_H_

#include "cnode/Message.h"

namespace resip
{

class DumCommand;

}
namespace messaging
{

class Message;

}

namespace utilities
{

class MessageTranslator
{
public:
	static resip::DumCommand* fromSupervisorMessage(messaging::Message& message);
	static cnode::Message::Ptr fromStackMessage(messaging::Message& message);
};

} /* namespace utilities */

#endif /* MESSAGETRANSLATOR_H_ */
