#include "cchat_server.h"

CChatServer::CChatServer()
    :CServerSocket(12345,10)
{
    this->isRun =true;
    memset(&MsgInfo,0,sizeof(MsgInfo));

    //添加Epollevent
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.data.fd = GetSocketfd();
    event.events = EPOLLIN;
    Epoll.EpollEventAdd(&event);
    pthread_mutex_init(&mutex,NULL);
    //Epoll.EpollEventAdd(GetSocketfd(),EPOLLIN);
    poll = new ThreadPoll(10);
}

CChatServer::~CChatServer()
{
    delete poll;
    this->OnlineUser.clear();
    pthread_mutex_destroy(&mutex);
}

void CChatServer::run()
{
    int num,res,accept_fd;
    int evnetsNum = Epoll.getEvnetsNum();
    struct epoll_event *events = Epoll.GetEvents();
    while(isRun&& !GetIsError())
    {
        num = Epoll.EpollWait();
        if (num < 0)
        {
            perror("CChatServer EpollWait error :");
            break;
        }
        for(int i = 0;i < num;i++)
        {
            //如果是socket_fd事件
            if(events[i].data.fd == GetSocketfd())
            {
                accept_fd = accept(GetSocketfd(),NULL,NULL);
                if(accept_fd < 0)
                {
                    perror("accpet error :");
                    break;
                }
                printf("acceptfd = %d \n",accept_fd);
                Epoll.EpollEventAdd(accept_fd,EPOLLIN);
                if(res == -1)
                {
                    perror("epoll ctl error : ");
                    break;
                }
            }
            //其他的EPOLLIN事件
            else if(events[i].events & EPOLLIN)
            {
                pthread_mutex_lock(&mutex);
                memset(&MsgInfo,0,sizeof(MsgInfo));
                res = read(events[i].data.fd,&MsgInfo,sizeof(MsgInfo));
                pthread_mutex_unlock(&mutex);
                if(res == -1)
                {
                    printf("error : %d   %d \n",i,events[i].data.fd);
                    perror("read error :");
                    break;
                }
                //client退出
                else if(res == 0)
                {
                    Epoll.EpollEventDel(events[i].data.fd,EPOLLIN);
                    if(res == -1)
                    {
                        perror("epoll ctl error : ");
                        break;
                    }
                    for(vector <struct Online>::iterator it = OnlineUser.begin();it != OnlineUser.end();it++)
                    {
                        if(it->accpet_fd == events[i].data.fd)
                        {
                            OnlineUser.erase(it);
                            break;
                        }
                    }
                    close(events[i].data.fd);
                }
                //读取到消息包
                else if(res == sizeof(MsgInfo))
                {
                    printf("读取到消息包\n");
                    MyTask * mytask = new MyTask(this,accept_fd);
                    poll->AddTask(mytask);
                }
            }

        }
        
    }
}


vector <struct Online> CChatServer::GetVector()
{
    return this->OnlineUser;
}

vector <struct Online>  *CChatServer::GetpVector()
{
    return &this->OnlineUser;
}


struct MsgInfo * CChatServer::GetMsg()
{
    return &this->MsgInfo;
}