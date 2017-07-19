/*************************************************************************
	> File Name: math_cgi.c
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Tue 18 Jul 2017 09:26:44 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define SIZE 1024

static void math_cal(char buff[])
{
	printf("cgi is running! \n");
	char* argv[3];
	char* start = buff;
	int i = 0;
	while(*start)
	{
		if(*start == '=')
		{
			start++;
			argv[i++] = start;
			continue;
		}
		else if(*start == '&')
		{
			*start = '\0';
		}
		start++;
	}

	argv[i] = NULL;
	int data1 = atoi(argv[0]);
	int data2 = atoi(argv[1]);
	printf("<html>");
	printf("<body>");
	printf("<h2>%d + %d = %d</h2><br/>", data1, data2, data1+data2);
	printf("<h2>%d - %d = %d</h2><br/>", data1, data2, data1-data2);
	printf("<h2>%d * %d = %d</h2><br/>", data1, data2, data1*data2);
	printf("<h2>%d / %d = %d</h2><br/>", data1, data2, data1/data2);
	printf("<h2>%d %% %d = %d</h2><br/>", data1, data2, data1%data2);
	printf("</body>");
	printf("</html>");
}

int main()
{
	char* method = NULL;
	char* arg_string = NULL;
	char* content_len = NULL;
	char buff[1024];

	method = getenv("METHOD");
	if(method && strcasecmp(method, "GET") == 0)
	{
		arg_string = getenv("QUERY_STRING");
		if(!arg_string)
		{
			printf("get method GET arg error!\n");
			return 1;
		}
		strcpy(buff, arg_string);
	}
	else if(method && strcasecmp(method, "POST") == 0)
	{
		content_len = getenv("CONTENT_LENGTH");
		if(!content_len)
		{
			printf("get method POST content_length error!\n");
			return 2;
		}
		int i = 0;
		char c = 0;
		int nums = atoi(content_len);
		for(; i < nums ; i++)
		{
			read(0, &c, 1);
			buff[i] = c;
		}
		buff[i] = '\0';
	}
	else
	{
		printf("get method error!\n");
		return 1;
	}

	math_cal(buff);
	return 0;
}
