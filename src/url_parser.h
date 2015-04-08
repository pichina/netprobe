/**
 * @file url_parser.h
 * @brief 
 * @author freeCoder
 * @version You can read,copy and modify this code.It's all ok.
 * @date 2015-03-22
 */
#ifndef URL_PARSER_H
#define URL_PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

typedef struct url_parser_url {
	char *protocol;
	char *host;
	int port;
	char *path;
	char *query_string;
	int host_exists;
	char *host_ip;
} url_parser_url_t;

int parse_url(char *url, bool verify_host, url_parser_url_t *parsed_url) ;
int merge_url(url_parser_url_t* parsed_url,char *buffer,int len);

#endif
