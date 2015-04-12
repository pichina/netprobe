/**
 * @file url_parser_tool.c
 * @brief parser url
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "url_parser.h"



/**
 * @brief 
 *
 * @param argc
 * @param argv[]
 *
 * @return 
 */
int main(int argc,char *argv[])
{
    int ret = 0;
    char *url = NULL;
    struct cdn_http_request  *httpresq;
    struct cdn_http_response  *httpresp;
    struct addrinfo hints,*res,*p;
    int status;
    void *addr;
    char *ipver;
    char ipstr[INET6_ADDRSTRLEN];

    url_parser_url_t *parsed_url = NULL;
    if(argc == 2)
    {
        url = argv[1];
    }
    else
    {
		printf("Usage:%s http://xxxx.com\n",argv[0]);
		return 1;
    }
    parsed_url = (url_parser_url_t *)malloc(sizeof(url_parser_url_t));
    if(!parsed_url)
    {
        goto mem_free;
    }
	/* parse url */
    ret = parse_url(url,false,parsed_url);
    if(ret)
    {
        printf("URL:mast begin with protocol(such as http)\n");
        if(parsed_url)
        {
            free_parsed_url(parsed_url);
        }
        return 2;
    }
    if(!parsed_url->protocol||!parsed_url->host)
    {
        printf("URL:mast begin with protocol(such as http)\n");
        if(parsed_url)
        {
            free_parsed_url(parsed_url);
        }
        return 3;
    }
	printf("%s\n",parsed_url->host);

mem_free:
    if(parsed_url)
    {
        free_parsed_url(parsed_url);
    }
    return 0;
}
