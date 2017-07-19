/*************************************************************************
	> File Name: sql.h
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Tue 18 Jul 2017 11:49:30 AM CST
 ************************************************************************/
#ifndef _SQL_H_
#define _SQL_H_

#include<iostream>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>

class sql
{
	public:
		sql(const std::string &_user, const std::string &_ip, \
				const std::string &_passwd, const std::string &_db, \
				const int &_port);
		~sql();
		int connect();
		int insert(const std::string &name, const std::string &sex,\
				const std::string &school, const std::string &hobby);
		int select();
	private:
		MYSQL* conn;
		std::string user;
		std::string ip;
		std::string passwd;
		std::string db;
		int port;

};

#endif
