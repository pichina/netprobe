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


#define NETPROBE_DB_NAME "netprobe.db"

#define NETPROBE_TBL_DROP "DROP TABLE IF EXISTS netprobe"

#define NETPROBE_TBL_CREATE "CREATE TABLE IF NOT EXISTS netprobe( \
                        id INTEGER PRIMARY KEY AUTOINCREMENT, \
                        url TEXT, \
                        host TEXT, \
                        result TEXT, \
                        http_code INTEGER, \
                        namelookup_time REAL, \
                        connect_time REAL, \
                        pretransfer_time REAL, \
                        size_upload REAL, \
                        size_download REAL, \
                        speed_upload REAL, \
                        speed_download REAL \
                        );"

#define NETPROBE_TBL_INSERT  "INSERT INTO netprobe( \
                url,host,result,http_code, \
                namelookup_time,connect_time, \
                pretransfer_time,size_upload, \
                size_download,speed_upload, \
                speed_download) \
                values(?,?,?,?,?,?,?,?,?,?,?)"

static int sqlite_init(sqlite3 **pdb)
{
    sqlite3 *db;
    char *err_msg = NULL;
    int rc = 0;
    rc = sqlite3_open(NETPROBE_DB_NAME,&db);
    if(rc != SQLITE_OK)
    {
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
    rc = sqlite3_exec(db,NETPROBE_TBL_CREATE,0,0,&err_msg);
    if(rc != SQLITE_OK)
    {
        sqlite3_close(db);
        *pdb = NULL;
        return -3;
    }
    *pdb = db;
    return 0;
}
int sqlite_export(struct cdn_http_request *httpresq,struct cdn_http_response *httpresp)
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
    rc = sqlite3_prepare_v2(db,NETPROBE_TBL_INSERT,-1,&stmt,NULL);
    if(SQLITE_OK != rc)
    {
        ret = -2;
        goto clean_exit;
    }
    sqlite3_bind_text(stmt,1,httpresq->url,-1,NULL);
    sqlite3_bind_text(stmt,2,httpresq->host,-1,NULL);
    sqlite3_bind_text(stmt,3,httpresp->ret == PROBE_SUCCESS?"success":"failed",-1,NULL);
    sqlite3_bind_int(stmt,4,(int)httpresp->response_code);
    sqlite3_bind_double(stmt,5,httpresp->namelookup_time);
    sqlite3_bind_double(stmt,6,httpresp->connect_time);
    sqlite3_bind_double(stmt,7,httpresp->pretransfer_time);
    sqlite3_bind_double(stmt,8,httpresp->size_upload);
    sqlite3_bind_double(stmt,9,httpresp->size_download);
    sqlite3_bind_double(stmt,10,httpresp->speed_upload);
    sqlite3_bind_double(stmt,11,httpresp->speed_download);

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
