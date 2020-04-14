/*
 * StopUserAgent.h
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#ifndef STOPUSERAGENT_H_
#define STOPUSERAGENT_H_

#include "helper/Message.h"

namespace cnode
{

class StopUserAgent : public messaging::Message
{
public:
	StopUserAgent(bool restart = false);
	virtual ~StopUserAgent();

private:
	bool mRestart;
};

} /* namespace cnode */

#endif /* STOPUSERAGENT_H_ */
