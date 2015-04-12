#!/bin/bash
################################################
#
# file io_sqlite.c
# brief 
# author freeCoder
# version You can read,copy and modify this code.It's all ok.
# date 2015-03-22
#
###############################################


GETHTTP="gethttp"
GETCDN="getCDN"
PING="mping"

SERVER=""
DBNAME="netprobe.db"

if [ $# != 1 ]
then
	echo "Usage $0  url"
	exit 1
fi
URL=$1
HOST=$(./url_parser_tool  $URL)
if [ $? != 0 ]
then
	echo "ok"
fi
echo "$URL"
echo "host:$HOST"
echo "###### BEGIN  netprobe ######"
$GETHTTP $URL
$GETCDN $URL
$PING $HOST
echo "###### FINISH netprobe ######"

