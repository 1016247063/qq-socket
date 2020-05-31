#include "thread_poll.h"


ThreadPoll::ThreadPoll(int maxThread,int createThread)
{
    this->ThreadNum = createThread;
    this->CreateThread = createThread;
    this->MaxThread = maxThread;
    this->ActiveThread = 0;
    this->isRun = true;
    if(maxThread < createThread)
    {
        this->MaxThread = this->ThreadNum;
    }

    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutexActive,NULL);
    pthread_cond_init(&cond,NULL);
    
    this->phtread_id = new pthread_t[maxThread];
    for(int i = 0 ;i < ThreadNum;i++)
    {
        //pthread_create(&phtread_id[i],NULL,threadFunc,this);
        NThread *  nthreadCreate = new NThread(threadFunc,this);
        NThreadList.push_back(nthreadCreate);
        phtread_id[i] = nthreadCreate->getThreadId();
        nthreadCreate->start();
        cout<<"pthread "<<i<<" id : "<<phtread_id[i]<<endl;
    }
}

ThreadPoll::~ThreadPoll()
{
    delete phtread_id;
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexActive);
    pthread_cond_destroy(&cond);
    // for(auto it = NThreadList.begin();it !=NThreadList.end();it++)
    // {
    //     delete *it;
    //     NThreadList.erase(it);
    // }
    NThreadList.clear();
    
}

void * ThreadPoll::threadFunc(void * p)
{
    ThreadPoll *poll = (ThreadPoll *)p;
    while (1)
    {
        //判断线程退出
        if(!poll->isRun)
        {
            break;
        }

        pthread_mutex_lock(&poll->mutex);
        //如果任务列表为空，就进入等待队列
        while(poll->TaskList.size() == 0)
        {
            pthread_cond_wait(&poll->cond,&poll->mutex);
        }
        poll->ChangeActiveThread(ADD);
        auto it = poll->TaskList.begin();
        Task * getTask = *it;
        poll->TaskList.erase(it);
        pthread_mutex_unlock(&poll->mutex);
        
        cout<<"pthread task run ,thread id = "<<pthread_self()<<endl;
        getTask->run();
        poll->ChangeActiveThread(DEL);
    }
    
}

void ThreadPoll::AddTask(Task * Addtask)
{

    pthread_mutex_lock(&mutex);

    //如果任务数大于最大线程的2倍自动丢弃
    if(TaskList.size() <= 2*MaxThread)
    {
        TaskList.push_back(Addtask);
        pthread_cond_signal(&cond);
    } 
    else
    {
        printf("任务数量太大，舍弃\n");
    }
    
    
    //存在任务为可用的2倍就创建 createThread数量的线程
    if((this->MaxThread != this->TaskList.size()) &&(this->TaskList.size() >= 2*this->GetSleepThreadNum()))
    {
        int create = 0;
        if(this->ThreadNum + this->CreateThread > this->MaxThread)
        {
            create = this->MaxThread-this->ThreadNum;
        }
        else
        {
            create = this->CreateThread;
        }

        for(int i = 0 ;i < create;i++)
        {
            //pthread_create(&phtread_id[this->ThreadNum],NULL,threadFunc,this);
            NThread *  nthreadCreate = new NThread(threadFunc,this);
            NThreadList.push_back(nthreadCreate);
            phtread_id[this->ThreadNum] = nthreadCreate->getThreadId();
            nthreadCreate->start();
            cout<<"pthread "<<i+this->ThreadNum<<" id : "<<phtread_id[this->ThreadNum]<<endl;
        }
        this->ThreadNum += create;
    }
    pthread_mutex_unlock(&mutex);
}

void ThreadPoll::Destroy()
{
    this->isRun = false;
}

void ThreadPoll::ChangeActiveThread(ThreadCtrl ctrl)
{
    if(ctrl == ADD)
    {
        pthread_mutex_lock(&mutexActive);
        ActiveThread++;
        pthread_mutex_unlock(&mutexActive);
    }
    else if(ctrl == DEL)
    {
        pthread_mutex_lock(&mutexActive);
        ActiveThread--;
        pthread_mutex_unlock(&mutexActive);
    }
}

int ThreadPoll::GetSleepThreadNum()
{
    return this->ThreadNum - this->ActiveThread;
}