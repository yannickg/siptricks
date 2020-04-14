/*
 * Shutdown.cpp
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#include "Shutdown.h"

namespace cnode
{

Shutdown::Shutdown() :
		messaging::Message(messaging::Message::Supervisor, messaging::Message::Shutdown)
{
}

Shutdown::~Shutdown()
{
}

} /* namespace cnode */
