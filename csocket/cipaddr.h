#ifndef _CIPADDR_H_
#define _CIPADDR_H_
#include <arpa/inet.h>
#include <stdio.h>
class CIpaddr
{
private:
    struct sockaddr_in addr;
public:
    CIpaddr(unsigned short int port,unsigned short int family,char * ip =NULL);
    ~CIpaddr();
    struct sockaddr_in * GetAddr();
    int getAddrSize();
};

#endif //_CIPADDR_H_