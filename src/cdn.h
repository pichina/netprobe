/*
 * =====================================================================================
 *
 *       Filename:  cdn.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年03月20日 23时38分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef CDN_H
#define CDN_H

#define NETPROBE_URL        "http://bcs.duapp.com/wifiauth/data/data_1M"
#define NETPROBE_USERAGENT  "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36"
#define MAX_URL_LEN 1024
#define MAX_HOST_LEN 128
#define ERROR_MSG_LEN 1024
enum NETPROBE_CODE
{
    PROBE_SUCCESS,
    PROBE_FAILED
};
struct cdn_http_request{
    char host[MAX_HOST_LEN];
    char url[MAX_URL_LEN];
};
struct cdn_http_response{
    enum NETPROBE_CODE ret;
    long response_code;
    double total_time; 
    double namelookup_time;
    double connect_time;
    double pretransfer_time;
    double size_upload;
    double size_download;
    double speed_upload;
    double speed_download;

    char error[ERROR_MSG_LEN];
};

extern int console_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp);
extern int sqlite_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp);

#endif
