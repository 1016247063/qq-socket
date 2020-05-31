#ifndef _NTHREAD_H_
#define _NTHREAD_H_

#include <pthread.h>

class NThread
{
public:
    typedef void *(threadFun_t)(void *arg);
    NThread(threadFun_t * Fun,void *arg);
    ~NThread();
    void start();                               //线程启动
    void join();                                //等待线程结束
    static void * threadRun(void * arg);        //线程执行函数
    pthread_t getThreadId();                    //获取线程id
    bool getisStart();                          //获取是否运行状态

private:
    bool isStart;                               //判断线程是否执行
    threadFun_t * Fun;                          //user的线程函数
    void * argv;                                //arg参数
    pthread_t pthreadId;                        //线程id
};


#endif //_NTHREAD_H_