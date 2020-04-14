/*
 * MessageTranslator.cpp
 *
 *  Created on: 2014-05-11
 *      Author: anniel1
 */

#include "MessageTranslator.h"
#include "NewSession.h"
#include <erl_interface.h>
#include <sstream>
#include <map>
#include <string>

static char format[] = ("{new_session, ~s, ~w}");

//
// Definition of an sip message record
//
// Request
// {
//     {
//        request,
//           {
//               method, uri, sip_version
//           }
//     },
//     ... (Headers/Body)
// }

// Response
// {
//     {
//        response,
//           {
//               code, reason
//           }
//     },
//     ... (Headers/Body)
// }

// Headers/Body
// {
//     {
//        headers,
//           [
//               {
//                   to,   <<sip:user@host>>
//               },
//               {
//                  from, <<sip:user@host>>
//               },
//               {
//                   ...,  <<...>>
//               }
//           ]
//     },
//     {
//        body, <<body>>
//     }
// }

namespace utilities
{

static bool   initialized  = false;
static ETERM* requestAtom  = NULL;
static ETERM* responseAtom = NULL;
static ETERM* headersAtom  = NULL;
static ETERM* bodyAtom     = NULL;

enum AtomType
{
	Request,
	Response,
	Headers,
	Body
};

static const char MessageDelimiter[]     = {"\r\n"};
static const char HeaderDelimiter[]      = {": "};
static const char ReqRespLineDelimiter[] = {" "};
static const char BodyDelimiter[]        = {"\r\n\r\n"};

resip::DumCommand* MessageTranslator::fromSupervisorMessage(messaging::Message& message)
{
	resip::DumCommand* command;
	return command;
}

cnode::Message::Ptr fromNewSessionStackMessage(messaging::Message& message);
cnode::Message::Ptr MessageTranslator::fromStackMessage(messaging::Message& message)
{
	cnode::Message::Ptr ptr;
	switch (message.type())
	{
	case messaging::Message::NewSession:
		ptr = fromNewSessionStackMessage(message);
		break;
	default:
		break;
	}

	return ptr;
}

char stripDash(char in)
{
	if (in == '-')
		return '_';

	return in;
}

std::string& formatAtom(const std::string& in, std::string& out)
{
	out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);
	std::transform(out.begin(), out.end(), out.begin(), stripDash);
	return out;
}

ETERM* createHeaderTuple(const std::pair<std::string, std::string>& header)
{
	ETERM* array[2];
	std::string stringAtom;
	array[0] = erl_mk_atom(formatAtom(header.first, stringAtom).c_str());
	array[1] = erl_mk_binary(header.second.c_str(), header.second.size());

	return erl_mk_tuple(array, 2);
}

std::pair<std::string, std::string>& splitHeader(const std::string& in, std::pair<std::string, std::string>& out)
{
	std::string header(in);
	std::string delimiter = HeaderDelimiter;

	size_t pos = 0;
	std::string token;
	while ((pos = header.find(delimiter)) != std::string::npos)
	{
	    if ((token = header.substr(0, pos)).empty())
	    {
	    	break;
	    }
	    header.erase(0, pos + delimiter.length());
	}

	out.first = token;
	out.second = header;
	return out;
}

inline bool init()
{
	if (!initialized)
	{
		requestAtom  = erl_mk_atom("request");
		responseAtom = erl_mk_atom("response");
		headersAtom  = erl_mk_atom("headers");
		bodyAtom     = erl_mk_atom("body");
		initialized  = true;
	}

	return initialized;
}

ETERM* atomFromType(AtomType atomType)
{
	init();

	ETERM* term = NULL;
	switch (atomType)
	{
	case Request:
		term = requestAtom;
		break;
	case Response:
		term = responseAtom;
		break;
	case Headers:
		term = headersAtom;
		break;
	case Body:
		term = bodyAtom;
		break;
	default:
		break;
	}
	return term;
}

ETERM* tupleFromReqRespLine(const std::string& in)
{
	std::string reqRespLine(in);
	std::string delimiter = ReqRespLineDelimiter;

	int i = 0;
	size_t pos = 0;
	std::string token;
	std::string parts[3];
	while ((pos = reqRespLine.find(delimiter)) != std::string::npos)
	{
	    if ((token = reqRespLine.substr(0, pos)).empty())
	    {
	    	break;
	    }
	    parts[i++] = token;
	    reqRespLine.erase(0, pos + delimiter.length());
	}

	bool isRequest = reqRespLine.size();
	if (isRequest)
	{
		parts[i++] = reqRespLine;
	}

	ETERM* arrayReqRespLine[i];
	for (int j=0; j<i; ++j)
	{
		arrayReqRespLine[j] = erl_mk_binary(parts[j].c_str(), parts[j].size());
	}
	ETERM* reqRespTuple = erl_mk_tuple(arrayReqRespLine, i);

	ETERM* arrayReqResp[2];
	arrayReqResp[0] = atomFromType(isRequest ? Request : Response);
	arrayReqResp[1] = reqRespTuple;

	return erl_mk_tuple(arrayReqResp, 2);
}

int extractContentLength(const std::pair<std::string, std::string>& header, int& contentLength)
{
	std::cout << header.first << std::endl;
	if (contentLength == 0 && header.first == "Content-Length")
	{
		std::stringstream(header.second) >> contentLength;
	}

	return contentLength;
}

ETERM* pushHeaderToList(const std::string& header, ETERM* headersList, int& contentLength)
{
    std::pair<std::string, std::string> pair;
    ETERM* headerTuple = createHeaderTuple(splitHeader(header, pair));
    headersList = erl_cons(headerTuple, headersList);
	extractContentLength(pair, contentLength);
    return headersList;
}

ETERM* splitHeaders(const std::string& in, int& contentLength)
{
	std::string headers(in);
	std::string delimiter = MessageDelimiter;

	size_t pos = 0;
	ETERM* tupleReqResp = NULL;
	// Extract Request/Response Line
	if ((pos = headers.find(delimiter)) != std::string::npos)
	{
	    std::string reqRespLine = headers.substr(0, pos);
	    tupleReqResp = tupleFromReqRespLine(reqRespLine);
		headers.erase(0, pos + delimiter.length());
	}

	std::string token;
	ETERM* headersList = erl_mk_empty_list();
	while ((pos = headers.find(delimiter)) != std::string::npos)
	{
	    token = headers.substr(0, pos);
	    headersList = pushHeaderToList(token, headersList, contentLength);
		headers.erase(0, pos + delimiter.length());
	}

    headersList = pushHeaderToList(headers, headersList, contentLength);

	ETERM* arrayHeaders[2];
	arrayHeaders[0] = atomFromType(Headers);
	arrayHeaders[1] = headersList;
	ETERM* tupleHeaders = erl_mk_tuple(arrayHeaders, 2);

    // Build final structure using reqRespTuple and tupleHeaders
	ETERM* arrayReqRespHeaders[2];
	arrayReqRespHeaders[0] = tupleReqResp;
	arrayReqRespHeaders[1] = tupleHeaders;
	return erl_mk_tuple(arrayReqRespHeaders, 2);
}

ETERM* recordFromSipMessage(const resip::SipMessage& sipMessage)
{
	std::stringstream stream;
	stream << sipMessage;

	std::string message = stream.str();
	std::string delimiter = BodyDelimiter;

	ETERM* record = NULL;

	size_t pos = 0;
	if ((pos = message.find(delimiter)) != std::string::npos)
	{
	    std::string headers = message.substr(0, pos);

		int contentLength = 0;
		ETERM* tupleHeaders = splitHeaders(headers, contentLength);
	    message.erase(0, pos + delimiter.length());

		// Body
	    std::string body = message;
	    assert(contentLength == body.size());
		ETERM* arrayBody[contentLength  > 0 ? 2 : 1];
		arrayBody[0] = atomFromType(Body);
		if (contentLength  > 0)
		{
			arrayBody[1] = erl_mk_binary(body.c_str(), body.size());
		}
		ETERM* tupleBody = erl_mk_tuple(arrayBody, contentLength  > 0 ? 2 : 1);

		ETERM* arrayRecord[2];
		arrayRecord[0] = tupleHeaders;
		arrayRecord[1] = tupleBody;
		record = erl_mk_tuple(arrayRecord, 2);
	}

	return record;
}

cnode::Message::Ptr fromNewSessionStackMessage(messaging::Message& message)
{
	gateway::NewSession& newSession = dynamic_cast<gateway::NewSession&>(message);

	ETERM* record = recordFromSipMessage(newSession.sipMessage());

	ETERM* msg = erl_format(
			format,
			newSession.callId().c_str(),
			record
			);

	cnode::Message::Ptr ptr(new cnode::Message(msg));
	return ptr;
}

} /* namespace utilities */
