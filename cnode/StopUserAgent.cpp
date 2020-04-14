/*
 * StopUserAgent.cpp
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#include "StopUserAgent.h"

namespace cnode
{

StopUserAgent::StopUserAgent(bool restart /* = false */) :
		messaging::Message(messaging::Message::Supervisor, messaging::Message::StopUserAgent),
		mRestart(restart)
{
}

StopUserAgent::~StopUserAgent()
{
}

} /* namespace cnode */
