/**
 * @file net.c
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <stdlib.h>
#include "net.h"

/**
 * @brief check the host is a ip addrss string
 *
 * @param host the host of the uri
 *
 * @return 
 */
int host_isipstr(char *host)
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

