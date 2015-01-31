#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <sys/time.h>


#define NETPROBE_URL        "http://bcs.duapp.com/wifiauth/data/data_1M"
#define NETPROBE_USERAGENT  "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36"


#define MAX_URL_LEN 1024
#define ERROR_MSG_LEN 1024

struct http_request{
	int id;
	char url[MAX_URL_LEN];
};

struct http_response{
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

int http_probe(struct http_request *httpresq,struct http_response *httpresp)
{
  CURL *curl_handle;
  CURLcode res;
  size_t filesize = 0;
  char *url = NULL;
  struct timeval start_tm,end_tm;
  unsigned long long elapsed = 0;
  unsigned long long  download_speed = 0;


  url = httpresq->url;


  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, NETPROBE_USERAGENT);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&filesize);
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
  curl_global_cleanup();
}

int main(int argc,char *argv[])
{
	char *url = NULL;
	struct http_request  *httpresq = NULL;
	struct http_response  *httpresp = NULL;



	if(argc == 2)
	{
		url = argv[1];
	}
	else
	{
		url = NETPROBE_URL;
	}
	httpresq = (struct http_request *)malloc(sizeof(struct http_request));
	if(!httpresq)
	{
		goto mem_free;
	}
	httpresp = (struct http_response *)malloc(sizeof(struct http_response));
	if(!httpresp)
	{
		goto mem_free;
	}
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

mem_free:
	free(httpresq);
	free(httpresp);
	return 0;
}
