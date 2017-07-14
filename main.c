/*************************************************************************
	> File Name: main.c
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Wed 12 Jul 2017 08:55:51 AM CST
 ************************************************************************/

#include"http.h"

void Usage(char* src)
{
	printf("%s : [local_ip] [local_port] \n", src);
}
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	//套接字网络部分
	int listen_sock = startup(argv[1], atoi(argv[2]));
	if(listen_sock < 0)
	{
		return 2;
	}

	while(1)
	{
		struct sockaddr_in client;
		int len = sizeof(client);

		int new_sock = accept(listen_sock, (struct sockaddr*)&client, &len);
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}

		printf("get a client : [ %s : %d ] \n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		//创建线程
		pthread_t id;
		pthread_create(&id, NULL, handler_request, (void*)new_sock);
		pthread_detach(id);
		
	}
	return 0;
}

