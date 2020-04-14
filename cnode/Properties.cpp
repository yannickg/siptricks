/*
 * Properties.cpp
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#include "Properties.h"

namespace cnode {

Properties::Properties(std::string peerName, std::string nodeName, std::string secretCookie) :
		mPeerName(peerName),
		mNodeName(nodeName),
		mSecretCookie(secretCookie)
{
}

} /* namespace cnode */
