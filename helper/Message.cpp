#include "Message.h"

namespace messaging
{

Message::Message(Source source, Type type) :
		mSource(source), mType(type)
{
}

Message::~Message()
{
}

Message::Source Message::source()
{
	return mSource;
}

Message::Type Message::type()
{
	return mType;
}

bool Message::shuttingDown()
{
	return (mType == Shutdown);
}

}
