/*
 * StartUserAgent.h
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#ifndef STARTUSERAGENT_H_
#define STARTUSERAGENT_H_

#include "helper/Message.h"

namespace cnode
{

class StartUserAgent : public messaging::Message
{
public:
	StartUserAgent();
	virtual ~StartUserAgent();
};

} /* namespace cnode */

#endif /* STARTUSERAGENT_H_ */
