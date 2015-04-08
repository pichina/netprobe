/**
 * @file getCDN.c
 * @brief CDN probe
 * @author freeCoder
 * @version 0.1
 * @date 2015-03-22
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <curl/curl.h>
#include <sys/time.h>

#include "url.h"
#include "net.h"
#include "url_parser.h"
#include "cdn.h"







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
        url = NETPROBE_URL;
    }
    httpresq = (struct cdn_http_request *)malloc(sizeof(struct cdn_http_request));
    if(!httpresq)
    {
        goto mem_free;
    }
    httpresp = (struct cdn_http_response *)malloc(sizeof(struct cdn_http_response));
    if(!httpresp)
    {
        goto mem_free;
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
        free(httpresq);
        free(httpresp);
        return -1;
    }
    if(!parsed_url->protocol||!parsed_url->host)
    {
        printf("URL:mast begin with protocol(such as http)\n");
        if(parsed_url)
        {
            free_parsed_url(parsed_url);
        }
        free(httpresq);
        free(httpresp);
        return -1;
    }
    strncpy(httpresq->host,parsed_url->host,MAX_HOST_LEN);
    memset(&hints,0x00,sizeof(hints));
    hints.ai_family =  AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	/* if host is a ip address ,one probe and return */
    status = host_isipstr(parsed_url->host);
    if(status != 0)
    {
        strncpy(httpresq->url,url,MAX_URL_LEN);
        http_probe(httpresq,httpresp);
		/* export result to console and sqlite */
        cdn_console_export(httpresq,httpresp);
        cdn_sqlite_export(httpresq,httpresp);
        goto mem_free;
    }
	/* use dns to convert host to ip address */
    status = getaddrinfo(parsed_url->host,NULL,&hints,&res);
    if(status  != 0)
    {
        if(parsed_url)
        {
            free_parsed_url(parsed_url);
        }
        free(httpresq);
        free(httpresp);
        return -2;
    }
    /* get from all ip of hosts */
    for(p = res;p != NULL ; p = p->ai_next)
    {

        /*  skip ipv6 */
        if(p->ai_family == AF_INET6)
            continue;
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
        inet_ntop(p->ai_family,addr,ipstr,sizeof(ipstr));
        /* change host url */
        strcpy(parsed_url->host,ipstr);
        ret = merge_url(parsed_url,httpresq->url,MAX_URL_LEN-1);
        if(!ret)
        {
            continue;
        }
        http_probe(httpresq,httpresp);
        //printf("%s\n",httpresp->error);
		/* export result to console and sqlite */
        cdn_console_export(httpresq,httpresp);
        cdn_sqlite_export(httpresq,httpresp);
    }
    freeaddrinfo(res);

mem_free:
    free(httpresq);
    free(httpresp);
    if(parsed_url)
    {
        free_parsed_url(parsed_url);
    }
    return 0;
}
