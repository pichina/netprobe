#include <stdlib.h>
#include "net.h"

int
host_isipstr(char *host)
{
    unsigned char buf[sizeof(struct in6_addr)];
    int domain,s;

    s = inet_pton(AF_INET,host,buf);
    if(s <= 0)
    {
        s = inet_pton(AF_INET6,host,buf);
    }
   
    if(s<=0)
        return 0;

    return 1;
}

