/**
 * @file io_console.c
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cdn.h"
#include "ping.h"

int cdn_console_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
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
    printf("|%30s|%30f|\n","total time",httpresp->total_time);
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

int ping_console_export(struct ping_task *task)
{
    int i = 0;
	int lost_count = 0;

    for(i = 0;i < TRY_COUNT;i++)
    {
		lost_count += task->lost[i];
    }
	printf("%d packets transmitted, %d packet loss\n",TRY_COUNT,lost_count);
}

