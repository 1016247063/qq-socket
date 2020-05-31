#ifndef _THREAD_POLL_H_
#define _THREAD_POLL_H_
#include <iostream>
#include <pthread.h>
#include <vector>
#include "task.h"
#include "nthread.h"

using namespace std;

/*
*   构造函数有两个参数，（最大线程，每次新创建的线程）
*   当任务容器里面的任务是  可用的两倍就新增 $(每次新创建的线程) 的数量
*/

class ThreadPoll
{
private:
    int ThreadNum;                      //当前线程数量
    int MaxThread;                      //最大线程数量
    int CreateThread;                   //每次创建的大小
    int ActiveThread;                   //当前活跃的线程数量
    pthread_mutex_t mutex;              
    pthread_mutex_t mutexActive;
    pthread_cond_t cond;
    vector<Task *> TaskList;            //任务向量列表
    pthread_t * phtread_id;             //线程id
    vector<NThread *> NThreadList;      //NThread 对象向量列表
    bool isRun;                         //是否运行的标志位

private:
    static void * threadFunc(void * p); //线程函数
    
public:
    enum ThreadCtrl{ADD,DEL};                               //添加或者减少计数      
    ThreadPoll(int maxThread,int createThread = 4);         //构造函数 maxThread为最大的线程createThread为每次最多创建的线程
    ~ThreadPoll();                                          //析构
    void AddTask(Task * Addtask);                           //添加任务到任务向量列表
    void Destroy();                                         //结束所有线程，正在运行的等待运行
    void ChangeActiveThread(ThreadCtrl ctrl);               //改变当前占用进程的累计计数
    int GetSleepThreadNum();                                //获得可用的进程数量
};


#endif //_THREAD_POLL_H_