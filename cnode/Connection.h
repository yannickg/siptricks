#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <erl_interface.h>
#include <ei.h>

namespace messaging
{
class Message;
}

namespace cnode
{

void init();

class Connection
{
public:
    Connection(const std::string& secretCookie, const std::string& peerName);
    ~Connection();

    bool connect(const std::string& peerName);
    void send(ETERM* term);
    bool receive(messaging::Message** message);
    bool receive(ErlMessage* msg);

private:
    bool handshake(ETERM* term);
    bool stopCommand(ETERM* term);

	int    mFd;                                  /* fd to Erlang node */
	ETERM* mPeerPid;
	bool mConnected;
};

}

#endif // CLIENT_H
