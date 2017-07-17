/*************************************************************************
	> File Name: http.h
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Wed 12 Jul 2017 08:55:25 AM CST
 ************************************************************************/
#ifndef __HTTP_H__
#define __HTTP_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/sendfile.h>

#define SIZE 1024
#define NOTICE 0
#define WARNING 1
#define FATAL 2


void print_log(const char* msg, int level);
int startup(const char* _ip, int _port);
void* handler_request(void *arg);
void echo_error(int fd, int errno_num);
int echo_www(int fd, const char* path, int size);
int exe_cgi(int fd, const char* method, const char* path, const char* query_string);

#endif
