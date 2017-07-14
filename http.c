/*************************************************************************
	> File Name: http.c
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Wed 12 Jul 2017 08:55:38 AM CST
 ************************************************************************/

#include"http.h"
#define MAX 10


int startup(const char* _ip, int _port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);
	local.sin_addr.s_addr = inet_addr(_ip);

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		perror("bind");
		return -2;
	}

	if(listen(sock, MAX) < 0)
	{
		perror("listen");
		return -3;
	}
	return sock;
}

//行获取 
static int getline(int fd, char* buff, int len)
{
	char c = '\0';
	int i = 0;
	while(c != '\n' && i < len-1)
	{
		ssize_t s = recv(fd, &c, 1, 0);
	}
}
void* handler_request(void *arg)
{
	int fd = (int)arg;

#ifdef _DEBUG_
	printf("###############################################");
	char buf[SIZE];
	int ret = -1;
	//请求报头+空行
	do
	{
		ret = getline(fd, buff, sizeof(buff));
	}while(ret > 0 && strcmp(buf, "\n"))
}
