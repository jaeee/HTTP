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
static int get_line(int fd, char* buff, int len)
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
		buff[i] = 0;
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
void echo_error(int fd, int errno_num)
{

}

int echo_www(int fd, const char* path, int size)
{
	int new_fd = open(path, O_RDONLY);
	if(new_fd < 0)
	{
		print_log("open file error!", FATAL);
		return 404;
	}

	const char* echo_line = "HTTP/1.0 200 OK\r\n";
	send(fd, echo_line, strlen(echo_line), 0);
	const char* blank_line = "\r\n";
	send(fd, blank_line,strlen(blank_line), 0);

	if(sendfile(fd, new_fd, NULL, size) < 0)
	{
		print_log("send file error!", FATAL);
		return 200;
	}
	close(new_fd);
}

int exe_cgi(int fd, const char* method, const char* path, const char* query_string)
{
	int content_len = -1;
	char METHOD[SIZE/10];
	char QUERY_STRING[SIZE];
	char CONTENT_LENGTH[SIZE];
	if(strcasecmp(method, "GET") == 0)
	{
		drop_header(fd);
	}
	else
	{
		char buff[SIZE];
		int ret = -1;
		do
		{
			ret = get_line(fd, buff, sizeof(buff));
			if(strncasecmp(buff, "Content-Length: ", 16) == 0)
			{
				content_len = atoi(&buff[16]);
			}
		}while(ret > 0 && strcmp(buff, "\n"));
		if(content_len == -1)
		{
			echo_error(fd, 401);
			return -1;
		}
	}
	printf("cgi: path: %s\n", path);
	int input[2];
	int output[2];
	if(pipe(input) < 0)
	{
		echo_error(fd, 401);
		return -2;
	}
	if(pipe(output) < 0)
	{
		echo_error(fd, 401);
		return -3;
	}
	const char* echo_line = "HTTP/1.0 200 OK\r\n";
	send(fd, echo_line, strlen(echo_line), 0);
	const char* type = "Content-Type:text/html;charset=ISO-8859-1\r\n";
	send(fd, type, strlen(type), 0);
	const char* blank_line="\r\n";
	send(fd, blank_line, strlen(blank_line), 0);

	pid_t id = fork();
	if(id < 0)
	{
		echo_error(fd, 501);
		return -2;
	}
	else if(id == 0)
	{//child
		close(input[1]);
		close(output[0]);
		sprintf(METHOD, "METHOD=%S", method);
		putenv(METHOD);
		if(strcasecmp(method, "GET") == 0)
		{
			sprintf(QUERY_STRING, "QUERY_STRING=%s", query_string);
			putenv(QUERY_STRING);
		}
		dup2(input[0], 0);
		dup2(output[1], 1);
		execl(path, path, NULL);
		exit(1);
	}
	else
	{
		close(input[0]);
		close(output[1]);

		int i = 0;
		char c = '\0';
		for(; i < content_len; i++)
		{
			recv(fd, &c, 1, 0);
			write(input[1], &c, 1);
		}

		while(1)
		{
			ssize_t s = read(output[0], &c, 1);
			if(s > 0)
			{
				send(fd, &c, 1, 0);
			}
			else
			{
				break;
			}
		}
		waitpid(id, NULL, 0);
		close(input[1]);
		close(output[0]);
	}
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
		errno_num = 501;
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
	while(isspace(buff[j]) && j < sizeof(buff))
	{
		j++;
	}
	//获取url
	i = 0;
	while(i < sizeof(url) && j < sizeof(buff) && !isspace(buff[j]))
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
		if(*query_string == '?')
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
