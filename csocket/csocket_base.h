#ifndef _CSOCKET_BASE_H_
#define _CSOCKET_BASE_H_
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>


//domain ipv4 和 ipv6
#define CSOCK_DOMAIN_IPV4   AF_INET
#define CSOCK_DOMAIN_IPV6   AF_INET6

class CSocketBase
{
public:
    CSocketBase(int domain);            //传入socket type
    ~CSocketBase();                 
    void DisableShowError();            //关闭错误提示
    void EnableShowError();             //打开错误提示
    void EnablePortReuse();             //打开端口复用
    bool GetIsError();                  //返回是否失败
    bool GetIsShowError();              //返回isShowError
    int GetSocketfd();                  //返回socketfd
    void ChangeError(bool isError);     //改变isError
    void ShowError(char * tips);        //展示错误信息（如果isShowError不为true则不展示）   
    virtual void run() = 0;             //执行函数
private:
    int socketfd;                       //socketfd
    bool isError;                       //判断socket是否错误
    bool isShowError;                   //是否打印错误信息（默认打印）

};

#endif //_CSOCKET_BASE_H_s