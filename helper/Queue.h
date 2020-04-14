#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <semaphore.h>
#include <pthread.h>

namespace messaging
{

class Message;

class Queue
{
public:
    Queue();
    ~Queue();

    void enqueue(Message& message);
    bool dequeue(Message** message);
    void stop();
    void clear();

private:
    std::deque<Message*> mQueue;
    sem_t mSemaphore;
    pthread_mutex_t mMutex;
};

}

#endif // QUEUE_H
