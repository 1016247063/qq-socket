#include "cepoll_base.h"
CEpollBase::CEpollBase()
{
    this->isError = false;
    this->isShowError = true;
    memset(&events,0,sizeof(events));
    //Since Linux 2.6.8, the size argu‐ment is ignored（30没有实际意义，大于0就好）
    epollfd = epoll_create(30);
    if(epollfd == -1)
    {
        ShowError("CEpollBase epoll_create:");
    }
}

CEpollBase::~CEpollBase()
{
    if(!GetIsError())
    {
        close(epollfd);
    }
}


void CEpollBase::DisableShowError()
{
    isShowError = false;
}
void CEpollBase::EnableShowError()
{
    isShowError  = true;
}

bool CEpollBase::GetIsError()
{
    return isError;
}

bool CEpollBase::GetIsShowError()
{
    return isShowError;
}

int CEpollBase::GetEpollfd()
{
    return epollfd;
}

void CEpollBase::ChangeError(bool isError)
{
    this->isError = isError;
}

void CEpollBase::ShowError(char * tips)
{
    if(GetIsShowError())
    {
        perror(tips);
    }
}

void CEpollBase::EpollEventAdd(struct epoll_event *event)
{
    int res = 0;
    res = epoll_ctl(epollfd,EPOLL_CTL_ADD,event->data.fd,event);
    if(res == -1)
    {
        ShowError("CEpollBase epoll_ctl error :");
    }
}   

void CEpollBase::EpollEventAdd(int fd,int epollEvents)
{
    int res = 0;
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.data.fd = fd;
    event.events = epollEvents;
    res = epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    if(res == -1)
    {
        ShowError("CEpollBase epoll_ctl error :");
    }
}

void CEpollBase::EpollEventDel(struct epoll_event *event)
{
    int res = 0;
    res = epoll_ctl(epollfd,EPOLL_CTL_DEL,event->data.fd,event);
    if(res == -1)
    {
        ShowError("CEpollBase epoll_ctl error :");
    }
}   

void CEpollBase::EpollEventDel(int fd,int epollEvents)
{
    int res = 0;
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.data.fd = fd;
    event.events = epollEvents;
    res = epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&event);
    if(res == -1)
    {
        ShowError("CEpollBase epoll_ctl error :");
    }
}

int CEpollBase::EpollWait()
{
    int res = 0;
    res = epoll_wait(epollfd,events,30,-1);
    if(res == -1)
    {
        ShowError("CEpollBase epoll_wait error");
    }
    return res;
}

struct epoll_event * CEpollBase::GetEvents()
{
    return events;
}

int CEpollBase::getEvnetsNum()
{
    return 30;
}