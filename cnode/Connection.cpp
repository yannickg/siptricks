#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <cstring>
#include <iostream>

#include "Connection.h"
#include "StartUserAgent.h"
#include "StopUserAgent.h"

#define BUFSIZE 1000

namespace // anonymous namespace
{

bool getLocalIp(std::string& ip)
{
	bool found = false;
	struct ifaddrs* ifaddr;
	struct ifaddrs* ifa;
	if (getifaddrs(&ifaddr) == -1)
	{
		return "";
	}

	char host[NI_MAXHOST];
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (
				(ifa->ifa_addr == NULL) ||
				(ifa->ifa_addr->sa_family == AF_INET6) ||
				(!strcmp(ifa->ifa_name, "lo"))
			) continue;

		if (!getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST))
		{
			found = true;
			ip = host;
			break;
		}
	}

	freeifaddrs(ifaddr);
	return found;
}

static bool IsInitialized = false;
bool initialized()
{
   bool inited = IsInitialized;
   if (!IsInitialized)
   {
     IsInitialized = true;
   }
   return inited;
}

static const char NodeNameDelimiter[] = {"@"};
static const char HostNameDelimiter[] = {"."};
bool splitNodeName(const std::string& in, std::string& aliveName, std::string& hostName)
{
	std::string nodeName(in);
	std::string delimiter = NodeNameDelimiter;

	bool found = false;
	size_t pos = 0;
	if ((pos = nodeName.find(delimiter)) != std::string::npos)
	{
	    aliveName = nodeName.substr(0, pos);
	    nodeName.erase(0, pos + delimiter.length());

	    delimiter = HostNameDelimiter;
		if ((pos = nodeName.find(delimiter)) != std::string::npos)
		{
			hostName = nodeName.substr(0, pos);
		}
		else
		{
			hostName = nodeName;
		}
		found = true;
	}

	return found;
}

void free_eterms(ErlMessage* msg)
{
	erl_free_term(msg->from);
	erl_free_term(msg->msg);
//	erl_free_term(msg->to);
}

// expecting messages in following formats:
//     {msg, payload}.
// ex: {start_user_agent, [{ip_address, port, tcp, ipv4},
//                         {ip_address, port, udp, ipv4}]}
messaging::Message* extract(ErlMessage* msg)
{
	messaging::Message* message = NULL;
	// TODO: based on type of message build the appropriate msg
	std::cout << "************************************" << std::endl;
	erl_print_term(stderr, msg->msg);
	std::cout << std::endl << "************************************" << std::endl;

	ETERM* start = erl_format("{~a, Payload}", "start_user_agent");
	ETERM* stop  = erl_format("{~a, Payload}", "stop_user_agent");
	if (erl_match(start, msg->msg))
	{
		ETERM* payload = erl_var_content(start, "Payload");
		erl_free_term(payload);

		message = new cnode::StartUserAgent();
	}
	else if (erl_match(stop, msg->msg))
	{
		ETERM* payload = erl_var_content(start, "Payload");
		message = new cnode::StopUserAgent();
	}

	erl_free_term(stop);
	erl_free_term(start);

	return message;

//
//
//
//	if (ERL_IS_TUPLE(msg->msg))
//	{
//		int size = erl_size(msg->msg);
//		for (int i=1; i<size; ++i)
//		{
//			ETERM* eTerm = erl_element(1, msg->msg);
//			ERL_ATOM_PTR(eTerm);
//			erl_free_term(eTerm);
//		}
//
//		ETERM *term1, *term2, *pattern1, *pattern2;
//	//    term1    = erl_format("{14,21}");
//	//    term2    = erl_format("{19,19}");
//	//    pattern1 = erl_format("{A,B}");
//	//    pattern2 = erl_format("{F,F}");
//		erl_match(term1, term2);
//
//		ETERM *fromp, *tuplep, *fnp, *argp, *resp;
//		fromp = erl_element(2, msg->msg);
//		tuplep = erl_element(3, msg->msg);
//
//		size = erl_size(tuplep);
//		fnp = erl_element(1, tuplep);
//		argp = erl_element(2, tuplep);
//
//		char* function = ERL_ATOM_PTR(fnp);
//		int argument = ERL_INT_VALUE(argp);
//		printf("function: %s(%i)\n", function, argument);
//
//		if (strncmp(ERL_ATOM_PTR(fnp), "foo", 3) == 0)
//		{
//		   //res = foo(ERL_INT_VALUE(argp));
//		}
//		else if (strncmp(ERL_ATOM_PTR(fnp), "bar", 3) == 0)
//		{
//		   //res = bar(ERL_INT_VALUE(argp));
//		}
//
//		erl_free_term(msg->from);
//		erl_free_term(msg->msg);
//		erl_free_term(tuplep);
//		erl_free_term(fnp);
//		erl_free_term(argp);
//	}
//	else
//	{
//
//	}
}

}

namespace cnode
{

void init()
{
   if (!initialized())
   {
      erl_init(NULL, 0);
   }
}

Connection::Connection(const std::string& nodeName, const std::string& secretCookie) :
    mFd(0),
    mPeerPid(0),
    mConnected(false)
{
	init();

	std::string ip;
	if (getLocalIp(ip))
	{
		std::string aliveName, hostName;
		splitNodeName(nodeName, aliveName, hostName);

		struct in_addr addr;
		addr.s_addr = inet_addr(ip.c_str());

		char* alive  = const_cast<char*>(aliveName.c_str());
		char* host   = const_cast<char*>(hostName.c_str());
		char* node   = const_cast<char*>(nodeName.c_str());
		char* cookie = const_cast<char*>(secretCookie.c_str());

		if (erl_connect_xinit(
				host,   // name of host (hostname)
				alive,  // alive name of node (alivename)
				node,   // name of the node (alivename@hostname)
				&addr,
				cookie,
				0       // instance of this cnode (sequential number)
				) == -1)
		{
			erl_err_quit("erl_connect_xinit");
		}
	}
}

Connection::~Connection()
{
    erl_free_term(mPeerPid);
}

bool Connection::connect(const std::string& peerName)
{
	bool connected = false;
	char* peer = const_cast<char*>(peerName.c_str());
	if ((mFd = erl_connect(peer)) >= 0)
	{
		fprintf(stderr, "READY\n");
		connected = true;
//		erl_err_quit("erl_connect");
	}

	return connected;
}

bool Connection::handshake(ETERM* term)
{
	bool matched = false;
	ETERM* match = erl_format("{~a, Pid}", "pid");
	if (erl_match(match, term))
	{
		mPeerPid = erl_var_content(match, "Pid");
		ETERM* nodePid = erl_mk_pid(
					erl_thisnodename(),
					mFd,
					0,
					erl_thiscreation()
				);
		ETERM* answer = erl_format("{~a, ~w}", "pid", nodePid);
		erl_send(mFd, mPeerPid, answer);
		erl_free_term(answer);
		matched = true;
		mConnected = true;
	}
	erl_free_term(match);

	return matched;
}

bool Connection::stopCommand(ETERM* term)
{
	bool matched = false;
	ETERM* match = erl_format("~a", "stop");
	if (erl_match(match, term))
	{
		matched = true;
	}
	erl_free_term(match);

	return matched;
}

bool Connection::receive(messaging::Message** message)
{
	ErlMessage msg;
	bool keepGoing = true;
	if ((keepGoing = receive(&msg)))
	{
		// TODO: the given msg argument will be passed to extract
		*message = extract(&msg);
	}
	free_eterms(&msg);

	return keepGoing;
}

bool Connection::receive(ErlMessage* msg)
{
	bool keepGoing = true;
	unsigned char buf[BUFSIZE];              /* Buffer for incoming message */
	while (true)
	{
		int got = erl_receive_msg(mFd, buf, BUFSIZE, msg);
		if (got == ERL_TICK)
		{
			/* ignore */
		}
		else if (got == ERL_ERROR)
		{
			keepGoing = false;
			break;
		}
		else
		{
			if (msg->type == ERL_REG_SEND)
			{
				if (handshake(msg->msg))
				{
					// do nothing
					continue;
				}
				else if (stopCommand(msg->msg))
				{
					// will schedule shutdown command soon.
					keepGoing = false;
				}
				else if (!mConnected)
				{
					// TODO: use proper exception
					throw;
				}
				break;
			}
		}
	}

	return keepGoing;
}

void Connection::send(ETERM* term)
{
   erl_send(mFd, mPeerPid, term);
}

}
