/*************************************************************************
	> File Name: mysql.cpp
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Tue 18 Jul 2017 11:49:37 AM CST
 ************************************************************************/
#include"sql.h"

sql::sql(const std::string &_user, const std::string &_ip,\
		const std::string &_passwd, const std::string &_db,\
		const int &_port):user(_user), ip(_ip), passwd(_passwd),\
						  db(_db), port(_port)
{
	conn = mysql_init(NULL); 
}
sql::~sql()
{
	mysql_close(conn);
}
int sql::connect()
{
	if(mysql_real_connect(conn, ip.c_str(), user.c_str(),\
				passwd.c_str(), db.c_str(), port, NULL, 0))
	{
		std::cout << "connect success!" << std::endl;
		return 0;
	}
	else
	{
		return -1;
	}
}
int sql::insert(const std::string &name, const std::string &sex,\
		const std::string &school, const std::string &hobby)
{
	std::string sql = "INSERT INTO student (name, sex, school, hobby) values('";
	sql += name;
	sql += "','";
	sql += sex;
	sql += "','";
	sql += school;
	sql += "','";
	sql += hobby;
	sql += "');";

	int ret = mysql_query(conn, sql.c_str());
	std::cout << sql << "ret: " << ret << std::endl;
}
int sql::select()
{
	std::string sql = "select * from student";
	int ret = mysql_query(conn, sql.c_str());
	if(ret == 0)
	{
		MYSQL_RES* res = mysql_store_result(conn);
		if(res)
		{
			int lines = mysql_num_rows(res);
			int cols = mysql_num_fields(res);
			std::cout << "lines: " << "cols: " << cols << std::endl;
			MYSQL_FIELD *fd = NULL;
			for(; fd = mysql_fetch_field(res) ;)
			{
				std::cout << fd->name << ' ';
			}

			std::cout << std::endl;

			int i = 0;
			for(; i < lines; i++)
			{
				MYSQL_ROW row = mysql_fetch_row(res);
				int j = 0;
				for(; i < lines; i++)
				{
					std::cout<< row[j] << ' ';
				}
				std::cout << std::endl;
			}
		}
		return 0;
	}
	return -1;
}
