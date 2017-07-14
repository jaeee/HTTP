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

//获得行消息
static int getline(int fd, char* buff, int len)
{
	char c = '\0';
	int i = 0;
	while(c != '\n' && i < len-1)
	{
		ssize_t s = recv(fd, &c, 1, 0);//recv前三个参数与read相同，返回值含义相同
		if(s > 0)
		{
			if(c == '\r') //平台不同换行符不同 一般三种情况 \r , \n, \r\n, \ra
			{
				recv(fd, &c, 1, MSG_PEEK);
				if(c == '\n')
				{
					recv(fd, &c, 1, 0);
				}
				else
				{
					c = '\n';
				}
			}
		}//到这里 所有的换行都统一为\n
		buf[i] = 0;
		return i;
	}

}
void drop_header(int fd)
{
	char buff[SIZE];
	int ret = -1;
	do
	{
		ret = get_line(fd, buff, sizeof(buff));
	}while(ret > 0 && strcmp(buff, "\n"));
}

int exe_cgi(int fd, const char* method, const char* path, const char* query_string)
{
	int content_len = -1;
	char METHOD[SIZE/10];
	char QUERY_STRING[SIZE];
	char CONTENT_LENGTH[SIZE];
}
//level:报警级别
void print_log(const char* msg, int level)
{
	const char *level_msg[] = {"NOTICE", "WARNING", "FATAL"};

	printf(" [%s][%s] \n", msg, level_msg[level]);
}
void* handler_request(void *arg)
{
	int fd = (int)arg;
	int errno_num = 200;
	int cgi = 0;
	char *query_string = NULL;//GET里URL变量

#ifdef _DEBUG_
	printf("###############################################\n");
	char buff[SIZE];
	int ret = -1;
	//请求报头+空行
	do
	{
		ret = getline(fd, buff, sizeof(buff));//getline返回的是文件描述符
	}while(ret > 0 && strcmp(buff, "\n"))
	printf("###############################################\n");

#else
	//1 method 2 url -> GET POST / url -> exit -> pri
	char method[SIZE/10];
	char url[SIZE];
	char path[SIZE];
	char buff[SIZE];
	int i = 0;
	int j = 0;
	//获取请求行
	if(get_line(fd, buff, sizeof(buff)) <= 0)
	{
		print_log("get request line error", FATAL);
		error_num = 501;
		goto end;
	}
    //获取方法
	while( i < sizeof(method) - 1 && j < sizeof(buff) && !isspace(buff[j]))
	{
		method[i] = buff[j];
		i++;
		j++;
	}
	method[i] = 0;
    //跳过你空格
	while(issapce(buff[j]) && j < sizeof(buff))
	{
		j++;
	}
	//获取url
	i = 0;
	while(i < sizeof(url) && j < sizeof(buff) && !issapce(buff[j]))
	{
		url[i] = buff[j];
		i++;
		j++;
	}
	url[i] = 0;
	printf("method : %s , url : %s ", method, url);
	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		//strcasecmp 忽略大小比较
		print_log("method is not ok!", FATAL);//获取方法失败
		errno_num = 501;
		goto end;
	}
    /* -----  CGI ----- */
	if(strcasecmp(method, "POST") == 0)
	{
		cgi = 1;
	}

	//url
	//GET后面有参数 ==》 CGI
	query_string = url;
	while(*query_string != 0)
	{
		//获取域
		if(query_string == '?')
		{
			cgi = 1;
			*query_string = '\0';
			query_string++;
			break;
		}
		query_string++;
	}
	/*************************************************/
	sprintf(path, "wwwroot%s", url);//????  // / /a/b/c
	if(path[strlen(path)-1] == '/')
	{
		strcat(path, "index.html");
	}

	printf("path: %s\n" , path);
	// /a/b/c.html
	struct stat st;
	//通过文件名获取文件信息，并保存buf所指的结构体stat中
	if(stat(path, &st) < 0)
	{
		print_log("path not found" , FATAL);
		errno_num = 404;
		goto end;
	}
	else
	{
		if(S_ISDIR(st.st_mode))
		{
			strcat(path, "/index.html");
		}
		else
		{
			if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) ||\
					(st.st_mode & S_IXOTH))
				cgi = 1;
		}
	}

	//path -> cgi
	if(cgi)
	{
		exe_cgi(fd, method, path, query_string);
	}
	else//GET-->normal
	{
		drop_header(fd);
		errno_num = echo_www(fd, path, st.st_size);
	}
end:
	echo_error(fd, errno_num);
	close(fd);
#endif

}
