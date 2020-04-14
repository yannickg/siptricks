#ifndef MESSAGETRANSPORT_H
#define MESSAGETRANSPORT_H

#include "Queue.h"

namespace messaging
{

class Callback;

class MessageTransport
{
public:
    MessageTransport();

    void postMessage(Message& message);
    void processMessages(Callback& callback);
    void shutdown();

private:
    Queue mQueue;
};

}

#endif // MESSAGETRANSPORT_H
