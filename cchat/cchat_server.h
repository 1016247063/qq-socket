#ifndef _CCHAT_SERVER_H_
#define _CCHAT_SERVER_H_
#include "cserver_socket.h"
#include <iostream>
#include <vector>
#include "cepoll_base.h"
#include "thread_poll.h"

using namespace std;

class CChatServer;
class MyTask;

/*
*用户信息结构体
*/
struct user
{
    int user_id;
    char userName[50];
};

//消息结构体
struct MsgInfo
{
    int MessageType;
    struct user sender;
    struct user recipient;
    char message[100];
};

//消息类型
enum MessageType{
    USERONLINE = 0,                                 //上线包
    MSGPUBLIC,                                      //群聊
    MSGSINGL,                                       //私聊
    FILESINGLE                                      //文件传输
    };                                    

/*
*在线用户数据
*/
struct Online
{
    int accpet_fd;			                        //存放accpet的文件描述符
    struct user user;		                        //存放用户数据信息
};

class CChatServer :public CServerSocket
{   
private:
    struct MsgInfo MsgInfo;                             //消息信息结构体
    vector <struct Online> OnlineUser;                  //在线用户向量
    CEpollBase Epoll;                                   //epoll结构体
    bool isRun;                                         //判断是否运行
    ThreadPoll *poll;                                    //线程池

public:
    CChatServer();
    ~CChatServer();
    virtual void run();                                 //运行函数
    vector <struct Online> GetVector();                 //获取向量
    vector <struct Online> *GetpVector();               //获取向量地址
    struct MsgInfo * GetMsg();                          //获取MsgInfo 
    pthread_mutex_t mutex;                              //锁
};



class MyTask :public Task
{
public:
    MyTask(CChatServer * chat,int acceptfd)
    {
        //加锁
        pthread_mutex_lock(&chat->mutex);
        OnlineUser = chat->GetVector();
        memset(&MsgInfo,0,sizeof(MsgInfo));
        memcpy(&MsgInfo,chat->GetMsg(),sizeof(MsgInfo));
        //解锁
        pthread_mutex_unlock(&chat->mutex);

        accept_fd = acceptfd;
        printf("MSG : %s\n",MsgInfo.message);
        pOnlineUser = chat->GetpVector();
    }
    ~MyTask()
    {
        this->OnlineUser.clear();
    }
    void run()
    {
        printf("MsgInfo.MessageType = %d \n",MsgInfo.MessageType);
        if(MsgInfo.MessageType == MessageType::USERONLINE)
        {
            struct Online OLUser;
            OLUser.accpet_fd = accept_fd;
            memcpy(&OLUser.user,&MsgInfo.sender,sizeof(MsgInfo.sender));
            pOnlineUser->push_back(OLUser);
                    
            //把一个人上线的消息发送给所有人
            for(vector <struct Online>::iterator it = OnlineUser.begin();it != OnlineUser.end();it++)
            {
                char Buffer[100] = {0};
                memset(&MsgInfo,0,sizeof(MsgInfo));
                sprintf(Buffer,"Online User : %s  ",OLUser.user.userName);
                MsgInfo.MessageType = MSGPUBLIC;
                memcpy(&MsgInfo.message,Buffer,strlen(Buffer));
                
                write(it->accpet_fd,&MsgInfo,sizeof(MsgInfo));
            }
        }
        //群聊消息
        else if(MsgInfo.MessageType == MessageType::MSGPUBLIC)
        {
            for(int i = 0; i < OnlineUser.size();i++)
            {
                printf("群聊消息\n");
                int res = write(OnlineUser[i].accpet_fd,&MsgInfo,sizeof(MsgInfo));
                if(res == -1)
                {
                    perror("write error : ");
                    break;
                }
            }
        }
        //私聊消息
        else if(MsgInfo.MessageType == MessageType::MSGSINGL ||MsgInfo.MessageType == MessageType::FILESINGLE )
        {
            for(int i = 0; i < OnlineUser.size();i++)
            {
                if(strcmp(OnlineUser[i].user.userName,MsgInfo.recipient.userName)==0)
                {
                    int res = write(OnlineUser[i].accpet_fd,&MsgInfo,sizeof(MsgInfo));
                    if(res == -1)
                    {
                        perror("write error : ");
                        break;
                    }
                    break;
                }
                
            }
        }
        delete this;
    }
private:
    vector <struct Online> OnlineUser;
    vector <struct Online> * pOnlineUser;
    struct MsgInfo MsgInfo;
    int accept_fd;
};


#endif //_CCHAT_SERVER_H_