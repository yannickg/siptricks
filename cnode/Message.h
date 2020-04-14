/*
 * Message.h
 *
 *  Created on: 2014-05-12
 *      Author: anniel1
 */

#ifndef CMESSAGE_H_
#define CMESSAGE_H_

#include <memory>
#include <erl_interface.h>

namespace cnode
{

class Message
{
public:
	typedef std::auto_ptr<Message> Ptr;

	Message(ETERM* msg);
	~Message();

	ETERM* erlangMessage();
	const ETERM* erlangMessage() const;

private:
	ETERM* mMsg;
};

} /* namespace cnode */

#endif /* CMESSAGE_H_ */
