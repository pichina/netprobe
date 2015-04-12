/**
 * @file gethttp.c
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <sys/time.h>
#include "cdn.h"


#define NETPROBE_URL        "http://bcs.duapp.com/wifiauth/data/data_1M"
#define NETPROBE_USERAGENT  "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36"


#define MAX_URL_LEN 1024
#define ERROR_MSG_LEN 1024



/**
 * @brief 
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 *
 * @return 
 */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  size_t* filesizep = (size_t*)userp;
  /* ignore content */
  *filesizep += realsize;
  return realsize;
}


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
	char *url = NULL;
	struct cdn_http_request  *httpresq = NULL;
	struct cdn_http_response  *httpresp = NULL;

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
	strncpy(httpresq->url,url,MAX_URL_LEN);
    http_probe(httpresq,httpresp);
    /* export result to console and sqlite */
    cdn_console_export(httpresq,httpresp);
    http_sqlite_export(httpresq,httpresp);


mem_free:
	free(httpresq);
	free(httpresp);
	return 0;
}
