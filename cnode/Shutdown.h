/*
 * Shutdown.h
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#ifndef SHUTDOWN_H_
#define SHUTDOWN_H_

#include "helper/Message.h"

namespace cnode
{

class Shutdown : public messaging::Message
{
public:
	Shutdown();
	virtual ~Shutdown();
};

} /* namespace cnode */

#endif /* SHUTDOWN_H_ */
