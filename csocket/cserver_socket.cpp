#include "cserver_socket.h"

CServerSocket::CServerSocket(unsigned short int port,int ListenNum)
    :CSocketBase(CSOCK_DOMAIN_IPV4),isRun(true)
{
    if(!GetIsError())
    {
        CIpaddr addr(port,CSOCK_DOMAIN_IPV4);
        SocketBind(addr);
        SocketListen(ListenNum);
    }
}

CServerSocket::CServerSocket(CIpaddr * addr,int ListenNum)
    :CSocketBase(CSOCK_DOMAIN_IPV4),isRun(true)
{
    if(!GetIsError())
    {
        SocketBind(*addr);
        SocketListen(ListenNum);
    }  
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::SocketBind(CIpaddr addr)
{
    EnablePortReuse();
    int res = 0;
    res = bind(GetSocketfd(),(__CONST_SOCKADDR_ARG)(addr.GetAddr()),addr.getAddrSize());
    if(res == -1)
    {
        ChangeError(true);
        ShowError("CServerSocket bind error :");
    }
    printf("bind\n");

}

void CServerSocket::SocketListen(int num)
{
    int res = 0;
    res = listen(GetSocketfd(),num);
    if(res == -1)
    {
        ChangeError(true);
        ShowError("CServerSocket listen Error:");
    }
    printf("listen\n");
}


void CServerSocket::run()
{
    if(this->RunFunc == NULL)
    {
        int acceptId;
        while (isRun)
        {
            acceptId = accept(GetSocketfd(),NULL,NULL);
            if(acceptId < 0)
            {
                ShowError("CServerSocket accept error:");
            }
            if(AcceptFunc != NULL)
                AcceptFunc(this);
        }
    }
    else
    {
        RunFunc(this);
    }
    
}


void CServerSocket::SetRunFunc(void (*RunFun)(void *))
{
    this->RunFunc = RunFun;
}

void CServerSocket::SetAcceptFunc(void (*AcceptFun)(void *))
{
    this->AcceptFunc = AcceptFun;
}