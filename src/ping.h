/**
 * @file ping.c
 * @brief CDN probe
 * @author freeCoder
 * @version 0.1
 * @date 2015-03-22
 */

#ifndef PING_H
#define PING_H 1

#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#define BUFFER_SIZE 1024
#define ICMP_HEADER_LENGTH 8
#define DATA_LENGTH (64 - ICMP_HEADER_LENGTH)
#define TRY_COUNT 10

struct ping_task{
    char hostname[1024];
    char send_ip[INET_ADDRSTRLEN];
    int data_length;
    unsigned int seq[10];
    int ttl[10];
    double  timeval[10];
};
#endif
