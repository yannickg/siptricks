/*
 * Message.cpp
 *
 *  Created on: 2014-05-12
 *      Author: anniel1
 */

#include "Message.h"

namespace cnode
{

Message::Message(ETERM* msg) :
		mMsg(msg)
{
}

Message::~Message()
{
	erl_free_term(mMsg);
}

ETERM* Message::erlangMessage()
{
	return mMsg;
}

const ETERM* Message::erlangMessage() const
{
	return mMsg;
}

} /* namespace cnode */
