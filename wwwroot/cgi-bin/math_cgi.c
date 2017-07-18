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

static void math_data(char* data)
{
	printf("cgi is running! \n");
	char* str[3];
	char* ptr = data;
	int i = 0;
	while(*ptr)
	{
		if(*ptr == '=')
		{
			str[i] = ptr+1;
			i++;
		}
		else if(*ptr == '&')
		{
			*ptr = '\0';
		}
		else
		{}
		ptr++;
	}

	str[i] = NULL;
	int data1 = atoi(str[0]);
	int data2 = atoi(str[1]);
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
	char method[SIZE];
	char content_data[SIZE];
	char content_len[SIZE];

	if(getenv("METHOD"))
	{
		strcpy(method, getenv("METHOD"));
	}
	else
	{
		perror("getenv method error");
		return 1;
	}

	printf("cgi method = %s \n" ,method);

	if(strcasecmp(method, "GET") == 0)
	{//GET
		printf("QUERY_STRING...\n");
		if(getenv("QUERY_STRING"))
		{
			strcpy(content_data, getenv("QUERY_STRING"));
		}
		else
		{
			perror("getenv query_string error");
			return 2;
		}
	}
	else
	{//POST
		printf("CONTENT_LEN....\n");
		if(getenv("CONTENT_LENGTH"))
		{
			strcpy(content_len, getenv("CONTENT_LENGTH"));
			int len = atoi(content_len);
			int i = 0;
			char ch = '\0';
			for(i = 0; i < len; i++)
			{
				read(0, &ch, 1);
				content_data[i] = ch;
			}
			content_data[i] = 0;
		}
		else
		{
			perror("getenv content_len error");
			return 3;
		}
	}
	math_data(content_data);
	return 0;
}
