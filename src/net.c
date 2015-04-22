/**
 * @file net.c
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <curl/curl.h>
#include <sys/time.h>

#include "net.h"
#include "url_parser.h"

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


/**
 * @brief callback function for get 
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 *
 * @return real size
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
 * @brief probe process function
 *
 * @param httpresq 
 * @param httpresp
 *
 * @return 
 */
int http_probe(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
{
    CURL *curl_handle;
    CURLcode res;
    size_t filesize;
    char *url;
    char *host;
    struct curl_slist *chunk;
    char *strhost;

    url = httpresq->url;
    host = httpresq->host;

    filesize = 0;
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
	/* very important */
	/* set Http header host:xxxxx */
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER,chunk);
    /* Get begin */
    res = curl_easy_perform(curl_handle);
    /* Get end */
	/* If failed,set all probe parameter to 0 */
    if(res != CURLE_OK)
    {
        httpresp->ret = PROBE_FAILED;
        strncpy(httpresp->error,curl_easy_strerror(res),ERROR_MSG_LEN);
        /* Probe begin */
        httpresp->response_code = 0;
        httpresp->total_time = 0;
        httpresp->namelookup_time = 0;
        httpresp->connect_time = 0;
        httpresp->pretransfer_time = 0;
        httpresp->size_upload = 0;
        httpresp->size_download = 0;
        httpresp->speed_upload = 0;
        httpresp->speed_download = 0;
    }
    else  /*Success,set all probe parameter of probe */
    {
        /* Probe begin */
        res = curl_easy_getinfo(curl_handle,CURLINFO_HTTP_CONNECTCODE,&httpresp->response_code);
        if(CURLE_OK != res)
            httpresp->response_code = 0;
		char *ip_str;
        res = curl_easy_getinfo(curl_handle,CURLINFO_PRIMARY_IP,&ip_str);
        if(CURLE_OK != res)
            httpresq->host[0] = 0;
		else
			strncpy(httpresq->host,ip_str,MAX_HOST_LEN);
        res = curl_easy_getinfo(curl_handle,CURLINFO_TOTAL_TIME,&httpresp->total_time);
        if(CURLE_OK != res)
            httpresp->total_time = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_NAMELOOKUP_TIME,&httpresp->namelookup_time);
        if(CURLE_OK != res)
            httpresp->namelookup_time = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_CONNECT_TIME,&httpresp->connect_time);
        if(CURLE_OK != res)
            httpresp->connect_time = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_PRETRANSFER_TIME,&httpresp->pretransfer_time);
        if(CURLE_OK != res)
            httpresp->pretransfer_time = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_SIZE_UPLOAD,&httpresp->size_upload);
        if(CURLE_OK != res)
            httpresp->size_upload = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_SIZE_DOWNLOAD,&httpresp->size_download);
        if(CURLE_OK != res)
            httpresp->size_download = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_SPEED_UPLOAD,&httpresp->speed_upload);
        if(CURLE_OK != res)
            httpresp->speed_upload = 0;
        res = curl_easy_getinfo(curl_handle,CURLINFO_SPEED_DOWNLOAD,&httpresp->speed_download);
        if(CURLE_OK != res)
            httpresp->speed_download = 0;

        /* Probe end */
        httpresp->ret = PROBE_SUCCESS;
    }
    curl_easy_cleanup(curl_handle);
    curl_slist_free_all(chunk);
    curl_global_cleanup();
    free(strhost);
	return 0;
}
