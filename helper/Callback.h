#ifndef MCALLBACK_H
#define MCALLBACK_H

namespace messaging
{

class Message;

class Callback
{
public:
	virtual ~Callback() { }
	virtual void operator()(Message& message) = 0;

protected:
	Callback() { }
};

}

#endif // MCALLBACK_H
