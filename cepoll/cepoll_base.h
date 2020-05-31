#ifndef _CEPOLL_BASE_H_
#define _CEPOLL_BASE_H_
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
class CEpollBase
{
private:
    int epollfd;                                                    //epollfd
    bool isError;                                                   //判断socket是否错误
    bool isShowError;                                               //是否打印错误信息（默认打印）
    struct epoll_event events[30];                                  //epoll事件列表

public:
    CEpollBase();
    ~CEpollBase();
    void DisableShowError();                                        //关闭错误提示
    void EnableShowError();                                         //打开错误提示
    bool GetIsError();                                              //返回是否失败
    bool GetIsShowError();                                          //返回isShowError
    int GetEpollfd();                                               //返回socketfd
    void ChangeError(bool isError);                                 //改变isError
    void ShowError(char * tips);                                    //展示错误信息（如果isShowError不为true则不展示） 
    void EpollEventAdd(struct epoll_event *event);                  //epoll 添加事件       
    void EpollEventAdd(int fd,int epollEvents);                     //epoll 添加事件
    void EpollEventDel(struct epoll_event *event);                  //epoll 添加事件       
    void EpollEventDel(int fd,int epollEvents);                     //epoll 添加事件
    int EpollWait();                                                //epoll wait
    struct epoll_event * GetEvents();                               //获取epollEvent
    int getEvnetsNum();                                             //获取Events大小

};

#endif //_CEPOLL_BASE_H_