/*
 * NewSession.h
 *
 *  Created on: 2014-05-08
 *      Author: anniel1
 */

#ifndef NEWSESSION_H_
#define NEWSESSION_H_

#include "helper/Message.h"
#include <resip/stack/SipMessage.hxx>
#include <string>

namespace resip
{
	class SipMessage;
}

namespace gateway
{

class NewSession: public messaging::Message
{
public:
	NewSession(const std::string& callId, const resip::SipMessage& sipMessage);
	virtual ~NewSession();

	const std::string& callId() const;
	const resip::SipMessage& sipMessage() const;

private:
	std::string mCallId;
	resip::SipMessage mSipMessage;
};

} /* namespace gateway */

#endif /* NEWSESSION_H_ */
