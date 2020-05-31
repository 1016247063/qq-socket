#include "nthread.h"

NThread::NThread(threadFun_t * Fun,void *arg)
{
    this->argv = arg;
    this->Fun = Fun;
    pthread_create(&this->pthreadId,NULL,this->threadRun,this);
}

NThread::~NThread()
{

}

bool NThread::getisStart()
{
    return this->isStart;
}

void * NThread::threadRun(void * arg)
{
    NThread * thread = (NThread *)arg;
    while(!thread->isStart);
    thread->Fun(thread->argv);
}

void NThread::start()
{
   this->isStart = true;
}

pthread_t NThread::getThreadId()
{
    return this->pthreadId;
}

void NThread::join()
{
    pthread_join(this->pthreadId,NULL);
}