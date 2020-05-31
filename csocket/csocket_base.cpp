#include "csocket_base.h"

CSocketBase::CSocketBase(int domain)
{
    isError = false;
    isShowError = true;
    socketfd = socket(domain,SOCK_STREAM,0);
    if (socketfd < 0)
    {
        isError = true;
        ShowError("CSocketBase create socket error :"); 
    }
    
}

CSocketBase::~CSocketBase()
{
    if(!isError)
    {
        close(socketfd);
    }
}


void CSocketBase::DisableShowError()
{
    isShowError = false;
}

void CSocketBase::EnableShowError()
{
    isShowError = true;
}


void CSocketBase::EnablePortReuse()
{
    if(!isError)
    {
        int res = 1;
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&res, sizeof(res));
    }
    
}

bool CSocketBase::GetIsError()
{
    return this->isError;
}

int CSocketBase::GetSocketfd()
{
    return this->socketfd;
}

void CSocketBase::ChangeError(bool isError)
{
    this->isError = isError;
}

bool CSocketBase::GetIsShowError()
{
    return this->isShowError;
}  

void CSocketBase::ShowError(char * tips)
{
    if (GetIsShowError())
    {
        perror(tips);
    }
}