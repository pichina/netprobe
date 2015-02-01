#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <curl/curl.h>
#include <sys/time.h>

#include "url.h"
#include "net.h"


#define NETPROBE_URL        "http://bcs.duapp.com/wifiauth/data/data_1M"
#define NETPROBE_USERAGENT  "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36"


#define MAX_URL_LEN 1024
#define MAX_HOST_LEN 128



#define ERROR_MSG_LEN 1024


struct cdn_http_request{
	int id;
    char host[MAX_HOST_LEN];
	char url[MAX_URL_LEN];
};

struct cdn_http_response{
	int ret;
	unsigned long long file_size;
	unsigned long long used_time;
	unsigned long long download_speed;
	char error[ERROR_MSG_LEN];
};



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  size_t* filesizep = (size_t*)userp;
  /* ignore content */
  *filesizep += realsize;
  return realsize;
}

int http_probe(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
{
  CURL *curl_handle;
  CURLcode res;
  size_t filesize;
  char *url;
  char *host;
  struct timeval start_tm,end_tm;
  unsigned long long elapsed;
  unsigned long long  download_speed;
  struct curl_slist *chunk;
  char *strhost;




  url = httpresq->url;
  host = httpresq->host;
  filesize = 0;
  elapsed = 0;
  download_speed = 0;
  strhost = (char *)malloc(MAX_HOST_LEN*2);
  snprintf(strhost,MAX_HOST_LEN*2,"Host: %s",host);
  chunk = NULL;
  chunk = curl_slist_append(chunk,strhost);
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, NETPROBE_USERAGENT);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&filesize);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER,0L);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST,0L);
  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER,chunk);
  /* Get begin */
  gettimeofday(&start_tm,NULL);
  res = curl_easy_perform(curl_handle);
  /* Get end */
  gettimeofday(&end_tm,NULL);
  elapsed = (end_tm.tv_sec - start_tm.tv_sec)*1000000LL+(end_tm.tv_usec - start_tm.tv_usec);
  if(res != CURLE_OK)
  {
	httpresp->ret = 1;
	strncpy(httpresp->error,curl_easy_strerror(res),ERROR_MSG_LEN);
  }
  else 
  {
	if(elapsed !=0)
	{
		download_speed = ((unsigned long long)filesize)*1000000/elapsed;
	}
	else
	{
		download_speed = 0;
	}
	httpresp->ret = 0;
	httpresp->file_size = filesize;
	httpresp->used_time = elapsed;
	httpresp->download_speed = download_speed;
  }
  curl_easy_cleanup(curl_handle);
  curl_slist_free_all(chunk);
  curl_global_cleanup();
  free(strhost);
}

int main(int argc,char *argv[])
{
	char *url = NULL;
	struct cdn_http_request  *httpresq;
	struct cdn_http_response  *httpresp;
    struct addrinfo hints,*res,*p;
    int status;
    void *addr;
    char *ipver;
    char ipstr[INET6_ADDRSTRLEN];
    char *new_url;

    url_data_t *data = NULL;
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
    data = url_parse(url);
    if(!data)
    {
        free(httpresq);
        free(httpresp);
        return -1;
    }
    strncpy(httpresq->host,data->host,MAX_URL_LEN);
    memset(&hints,0x00,sizeof(hints));
    hints.ai_family =  AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = host_isipstr(data->host);
    if(status != 0)
    {
        strncpy(httpresq->url,url,MAX_URL_LEN);
        http_probe(httpresq,httpresp);
        if(httpresp->ret)
        {
            printf("HTTP GET FAILED\n");
            printf("%s\n",httpresp->error);
        }	
        else
        {
            printf("HTTP GET Success\n");
            printf("filesize is: %llu\n",httpresp->file_size);
            printf("speed is: %llu\n",httpresp->download_speed);
        }
        goto mem_free;
    }
        
    status = getaddrinfo(data->host,NULL,&hints,&res);
    if(status  != 0)
    {
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
        strcpy(data->host,ipstr);
        new_url  =  url_data_merge(data);
        printf("%s\n",new_url);
        strncpy(httpresq->url,url,MAX_URL_LEN);
        http_probe(httpresq,httpresp);
        if(httpresp->ret)
        {
            printf("HTTP GET FAILED\n");
            printf("%s\n",httpresp->error);
        }	
        else
        {
            printf("HTTP GET Success\n");
            printf("filesize is: %llu\n",httpresp->file_size);
            printf("speed is: %llu\n",httpresp->download_speed);
        }
        free(new_url);
        new_url = NULL;
    }
    freeaddrinfo(res);

mem_free:
	free(httpresq);
	free(httpresp);
	return 0;
}
