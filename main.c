/*************************************************************************
	> File Name: main.c
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Wed 12 Jul 2017 08:55:51 AM CST
 ************************************************************************/

#include"http.h"

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	return 0;
}

