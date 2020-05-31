#include "cipaddr.h"

CIpaddr::CIpaddr(unsigned short int port,unsigned short int family,char * ip)
{
    if(ip!=NULL)
        addr.sin_addr.s_addr = inet_addr(ip);
    else
        addr.sin_addr.s_addr = INADDR_ANY;
    
    addr.sin_family = family;
    addr.sin_port = htons(port);
}

CIpaddr::~CIpaddr()
{

}


struct sockaddr_in * CIpaddr::GetAddr()
{
    return &addr;
}

int CIpaddr::getAddrSize()
{
    return sizeof(addr);
}