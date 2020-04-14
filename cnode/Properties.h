/*
 * Properties.h
 *
 *  Created on: 2014-06-01
 *      Author: anniel1
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>

namespace cnode {

class Properties
{
public:
	Properties(std::string peerName, std::string nodeName, std::string secretCookie);

	std::string mPeerName;
	std::string mNodeName;
	std::string mSecretCookie;
};

} /* namespace cnode */

#endif /* PROPERTIES_H_ */
