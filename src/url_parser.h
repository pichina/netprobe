#ifndef URL_PARSER_H
#define URL_PARSER_H
/*
 * =====================================================================================
 *
 *       Filename:  url_parser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年03月18日 21时24分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

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
