/**
 * @file io_sqlite.c
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "cdn.h"
#include "ping.h"


#define NETPROBE_DB_NAME "netprobe.db"

#define NETPROBE_TBL_HTTP_DROP "DROP TABLE IF EXISTS HTTP"
#define NETPROBE_TBL_CDN_DROP "DROP TABLE IF EXISTS CDN"
#define NETPROBE_TBL_PING_DROP "DROP TABLE IF EXISTS PING"


#define NETPROBE_TBL_HTTP_CREATE "CREATE TABLE IF NOT EXISTS HTTP( \
                        id INTEGER PRIMARY KEY AUTOINCREMENT, \
                        url TEXT, \
                        host TEXT, \
                        result TEXT, \
                        http_code INTEGER, \
						total_time REAL,   \
                        namelookup_time REAL, \
                        connect_time REAL, \
                        pretransfer_time REAL, \
                        size_upload REAL, \
                        size_download REAL, \
                        speed_upload REAL, \
                        speed_download REAL \
                        );"


#define NETPROBE_TBL_CDN_CREATE "CREATE TABLE IF NOT EXISTS CDN( \
                        id INTEGER PRIMARY KEY AUTOINCREMENT, \
                        url TEXT, \
                        host TEXT, \
                        result TEXT, \
                        http_code INTEGER, \
						total_time REAL,   \
                        namelookup_time REAL, \
                        connect_time REAL, \
                        pretransfer_time REAL, \
                        size_upload REAL, \
                        size_download REAL, \
                        speed_upload REAL, \
                        speed_download REAL \
                        );"

#define NETPROBE_TBL_PING_CREATE "CREATE TABLE IF NOT EXISTS PING( \
                        id INTEGER PRIMARY KEY AUTOINCREMENT, \
                        hostname TEXT, \
                        ip TEXT, \
                        data_length INTEGER, \
                        timeval INTEGER, \
						lost INTEGER \
                        );"

#define NETPROBE_TBL_HTTP_INSERT  "INSERT INTO HTTP( \
                url,host,result,http_code, \
                total_time,namelookup_time,connect_time, \
                pretransfer_time,size_upload, \
                size_download,speed_upload, \
                speed_download) \
                values(?,?,?,?,?,?,?,?,?,?,?,?)"


#define NETPROBE_TBL_CDN_INSERT  "INSERT INTO CDN( \
                url,host,result,http_code, \
                total_time,namelookup_time,connect_time, \
                pretransfer_time,size_upload, \
                size_download,speed_upload, \
                speed_download) \
                values(?,?,?,?,?,?,?,?,?,?,?,?)"

#define NETPROBE_TBL_PING_INSERT  "INSERT INTO PING( \
                hostname,ip,data_length,timeval,lost) \
                values(?,?,?,?,?)"

static int sqlite_init(sqlite3 **pdb)
{
    sqlite3 *db;
    char *err_msg = NULL;
    int rc = 0;
    rc = sqlite3_open(NETPROBE_DB_NAME,&db);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -1;
    }
#if 0
    rc = sqlite3_exec(db,NETPROBE_TBL_DROP,0,0,&err_msg);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -2;
    }
#endif
    rc = sqlite3_exec(db,NETPROBE_TBL_HTTP_CREATE,0,0,&err_msg);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -3;
    }
    rc = sqlite3_exec(db,NETPROBE_TBL_CDN_CREATE,0,0,&err_msg);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -3;
    }
    rc = sqlite3_exec(db,NETPROBE_TBL_PING_CREATE,0,0,&err_msg);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -3;
    }
    *pdb = db;
    return 0;
}

int http_sqlite_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int ret = 0;
    
    db = NULL;
    rc = sqlite_init(&db);
    if(SQLITE_OK != rc)
    {
        ret = -1;
        goto clean_exit;
    }
    rc = sqlite3_prepare_v2(db,NETPROBE_TBL_HTTP_INSERT,-1,&stmt,NULL);
    if(SQLITE_OK != rc)
    {
        ret = -2;
        goto clean_exit;
    }
    sqlite3_bind_text(stmt,1,httpresq->url,-1,NULL);
    sqlite3_bind_text(stmt,2,httpresq->host,-1,NULL);
    sqlite3_bind_text(stmt,3,httpresp->ret == PROBE_SUCCESS?"success":"failed",-1,NULL);
    sqlite3_bind_int(stmt,4,(int)httpresp->response_code);
    sqlite3_bind_double(stmt,5,httpresp->total_time);
    sqlite3_bind_double(stmt,6,httpresp->namelookup_time);
    sqlite3_bind_double(stmt,7,httpresp->connect_time);
    sqlite3_bind_double(stmt,8,httpresp->pretransfer_time);
    sqlite3_bind_double(stmt,9,httpresp->size_upload);
    sqlite3_bind_double(stmt,10,httpresp->size_download);
    sqlite3_bind_double(stmt,11,httpresp->speed_upload);
    sqlite3_bind_double(stmt,12,httpresp->speed_download);

    rc = sqlite3_step(stmt);
    if(SQLITE_OK != rc)
    {
        ret = -3;
        goto clean_exit;
    }
    ret = 0;
clean_exit:
    if(!ret)
    {
        printf("export to sqlite database "NETPROBE_DB_NAME);
    }
    if(db)
    {
        sqlite3_close(db);
        db = NULL;
    }
    return ret;
}


int cdn_sqlite_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int ret = 0;
    
    db = NULL;
    rc = sqlite_init(&db);
    if(SQLITE_OK != rc)
    {
        ret = -1;
        goto clean_exit;
    }
    rc = sqlite3_prepare_v2(db,NETPROBE_TBL_CDN_INSERT,-1,&stmt,NULL);
    if(SQLITE_OK != rc)
    {
        ret = -2;
        goto clean_exit;
    }
	
    sqlite3_bind_text(stmt,1,httpresq->url,-1,NULL);
    sqlite3_bind_text(stmt,2,httpresq->host,-1,NULL);
    sqlite3_bind_text(stmt,3,httpresp->ret == PROBE_SUCCESS?"success":"failed",-1,NULL);
    sqlite3_bind_int(stmt,4,(int)httpresp->response_code);
    sqlite3_bind_double(stmt,5,httpresp->total_time);
    sqlite3_bind_double(stmt,6,httpresp->namelookup_time);
    sqlite3_bind_double(stmt,7,httpresp->connect_time);
    sqlite3_bind_double(stmt,8,httpresp->pretransfer_time);
    sqlite3_bind_double(stmt,9,httpresp->size_upload);
    sqlite3_bind_double(stmt,10,httpresp->size_download);
    sqlite3_bind_double(stmt,11,httpresp->speed_upload);
    sqlite3_bind_double(stmt,12,httpresp->speed_download);

    rc = sqlite3_step(stmt);
    if(SQLITE_OK != rc)
    {
        ret = -3;
        goto clean_exit;
    }
    ret = 0;
clean_exit:
    if(!ret)
    {
        printf("export to sqlite database "NETPROBE_DB_NAME);
    }
    if(db)
    {
        sqlite3_close(db);
        db = NULL;
    }
    return ret;
}




int ping_sqlite_export(struct ping_task *task)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int ret = 0;
    double timeval_avg = 0;
	double lost_count = 0;
	double lost_avg = 0;
    int i = 0;

    for(i = 0;i < TRY_COUNT;i++)
    {
		if(!task->lost[i])
		{
			timeval_avg += task->timeval[i];
		}
		lost_count += task->lost[i];
    }
    timeval_avg  = timeval_avg/(TRY_COUNT-lost_count);
	lost_avg = lost_count/TRY_COUNT;
    db = NULL;
    rc = sqlite_init(&db);
    if(SQLITE_OK != rc)
    {
        ret = -1;
        goto clean_exit;
    }
    rc = sqlite3_prepare_v2(db,NETPROBE_TBL_PING_INSERT,-1,&stmt,NULL);
    if(SQLITE_OK != rc)
    {
        ret = -2;
        goto clean_exit;
    }
    sqlite3_bind_text(stmt,1,task->hostname,-1,NULL);
    sqlite3_bind_text(stmt,2,task->send_ip,-1,NULL);
    sqlite3_bind_int(stmt,3,task->data_length);
    sqlite3_bind_double(stmt,4,timeval_avg);
    sqlite3_bind_double(stmt,4,lost_avg);
	

    rc = sqlite3_step(stmt);
    if(SQLITE_OK != rc && SQLITE_DONE !=rc && SQLITE_ROW != rc)
    {
        ret = -3;
        goto clean_exit;
    }
    ret = 0;
clean_exit:
    if(!ret)
    {
        printf("export to sqlite database "NETPROBE_DB_NAME"\n");
    }
    if(db)
    {
        sqlite3_close(db);
        db = NULL;
    }
    return ret;
}
