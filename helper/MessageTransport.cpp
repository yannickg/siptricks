#include "MessageTransport.h"
#include "Callback.h"
#include "Message.h"

namespace messaging
{

MessageTransport::MessageTransport()
{
}

void MessageTransport::postMessage(Message& message)
{
    mQueue.enqueue(message);
}

void MessageTransport::processMessages(Callback& callback)
{
    for (;;)
    {
        Message* message = NULL;
        if (!mQueue.dequeue(&message))
        {
            break;
        }

        if (message)
        {
        	callback(*message);
        	if (message->shuttingDown())
        	{
        		break;
        	}

        	delete message;
        }
    }
}

void MessageTransport::shutdown()
{
    mQueue.stop();
}

}
