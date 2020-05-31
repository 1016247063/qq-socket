#ifndef _CSEVER_SOCKET_H_
#define _CSEVER_SOCKET_H_
#include "cipaddr.h"
#include "csocket_base.h"

class CServerSocket :public CSocketBase
{
private:
    void SocketBind(CIpaddr addr);                                      //bind函数，内部调用
    void SocketListen(int num);                                         //listen函数,内部调用
    void (*RunFunc)(void *);                                            //运行函数指针   
    void (*AcceptFunc)(void *);                                         //函数指针                                         

public:
    CServerSocket(unsigned short int port,int ListenNum);
    CServerSocket(CIpaddr * addr,int ListenNum);
    ~CServerSocket();
    virtual void run();                                                 //内部运行函数
    void SetRunFunc(void (*RunFun)(void *));                            //设置运行函数
    void SetAcceptFunc(void (*AcceptFun)(void *));                      //设置运行函数

private:
    bool isRun;                                                         //是否执行
};


#endif //_CSEVER_SOCKET_H_