#include "Queue.h"
#include "Message.h"

namespace messaging
{

Queue::Queue()
{
    sem_init(&mSemaphore, 1, 0);
    pthread_mutex_init(&mMutex, NULL);
}

Queue::~Queue()
{
    sem_destroy(&mSemaphore);
    pthread_mutex_destroy(&mMutex);
}

void Queue::enqueue(Message& message)
{
    pthread_mutex_lock(&mMutex);
    mQueue.push_back(&message);
    pthread_mutex_unlock(&mMutex);
    sem_post(&mSemaphore);
}

bool Queue::dequeue(Message** message)
{
    sem_wait(&mSemaphore);
    pthread_mutex_lock(&mMutex);
    bool success = false;
    if (mQueue.size())
    {
        *message = mQueue.front();
        mQueue.pop_front();
        success = true;
    }
    pthread_mutex_unlock(&mMutex);
    return success;
}

void Queue::stop()
{
    clear();
    sem_post(&mSemaphore);
}

void Queue::clear()
{
    pthread_mutex_lock(&mMutex);
    mQueue.clear();
    pthread_mutex_unlock(&mMutex);
}

}
