/*
 * NewSession.cpp
 *
 *  Created on: 2014-05-08
 *      Author: anniel1
 */

#include "NewSession.h"

namespace gateway
{

NewSession::NewSession(const std::string& callId, const resip::SipMessage& sipMessage) :
		Message(messaging::Message::Wire, messaging::Message::NewSession),
		mCallId(callId),
		mSipMessage(sipMessage)
{
}

NewSession::~NewSession()
{
}

const std::string& NewSession::callId() const
{
	return mCallId;
}

const resip::SipMessage& NewSession::sipMessage() const
{
	return mSipMessage;
}

} /* namespace gateway */
