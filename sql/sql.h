/*************************************************************************
	> File Name: mysql.h
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Tue 18 Jul 2017 11:49:30 AM CST
 ************************************************************************/
#ifndef _MYSQL_H_
#define _MYSQL_H_

#include<iostream>
#include<iostream>
#include<string>
#include<stdio.h>
#include<mysql.h>

class sql
{
	public:
		sql();
		~sql();
		int sql_connect();
		int sql_insert(const std::string& data);
		void sql_select(const std::string& data);
	private:
		MYSQL my_sql;

};

#endif
