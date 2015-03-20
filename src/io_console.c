/*
 * =====================================================================================
 *
 *       Filename:  io_console.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年03月20日 23时37分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cdn.h"

int console_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
{
    char table_header[132]={0,};
    char line_header[62]={0,};
    memset(table_header,'*',61);
    memset(line_header,'_',61);
    
    printf("|%61s|\n",table_header);
    printf("|%30s|%30s|\n","url",httpresq->url);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30s|\n","host",httpresq->host);
    printf("|%61s|\n",line_header);
    if(PROBE_FAILED == httpresp->ret)
    {
        printf("|%30s|%30s|\n","get result","failed");
        printf("|%61s|\n",line_header);
        return 0;
    }
    else
    {
        printf("|%30s|%30s|\n","get result","success");
        printf("|%61s|\n",line_header);
    }
    printf("|%30s|%30ld|\n","http code",httpresp->response_code);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","namelookup time",httpresp->namelookup_time);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","connect time",httpresp->connect_time);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","pretransfer time",httpresp->pretransfer_time);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","size upload",httpresp->size_upload);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","size download",httpresp->size_download);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","speed upload(kbyte/sec)",httpresp->speed_upload);
    printf("|%61s|\n",line_header);
    printf("|%30s|%30f|\n","speed download(kbyte/sec)",httpresp->speed_download);
    printf("|%61s|\n",line_header);
    return 0;
}

