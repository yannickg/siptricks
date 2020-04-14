/*
 * StartUserAgent.cpp
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#include "StartUserAgent.h"

namespace cnode
{

StartUserAgent::StartUserAgent() :
		messaging::Message(messaging::Message::Supervisor, messaging::Message::StartUserAgent)
{
}

StartUserAgent::~StartUserAgent()
{
}

} /* namespace cnode */
