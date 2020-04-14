#ifndef HMESSAGE_H
#define HMESSAGE_H

namespace messaging
{

class Message
{
public:
	enum Type
	{
		Shutdown,
		StartUserAgent,
		StopUserAgent,
		NewSession
	};

	enum Source
	{
		Supervisor,
		Wire
	};

    virtual ~Message();

    Source source();
    Type type();
    virtual bool shuttingDown();

protected:
    Message(Source source, Type type);

    Source mSource;
    Type mType;
};

}

#endif // HMESSAGE_H
