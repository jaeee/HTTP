/*************************************************************************
	> File Name: mysql.cpp
	> Author: liumin
	> Mail: 1106863227@qq.com 
	> Created Time: Tue 18 Jul 2017 11:49:37 AM CST
 ************************************************************************/
#include"sql.h"
sql::sql()
{
	mysql_init(&my_sql);//mysql.h 中提供的函数
}
sql::~sql()
{
	mysql_close(&my_sql);
}
int sql::sql_connect()
{
	bool ret = true;
	if(mysql_real_connect(&my_sql, "127.0.0.1", "root", NULL, "student_info", 3306, NULL, 0) == NULL)
	{
		std::cout << "connect failed..."<< std::endl;
		ret = false;
		goto end;
	}
	std::cout << "connect success ... " << std::endl;
end:
	return ret;
}
int sql::sql_insert(const std::string& data)
{
	std::string str = "insert into student(name, sex, school, hobby)values(";
	str += data;
	str += ")";
	mysql_query(&my_sql, str.c_str());
}
void sql::sql_select(const std::string& data)
{
	MYSQL_RES *res;
	std::string str = "select * from student";
	if(mysql_query(&my_sql, data.c_str()) == 0)
	{
		res = mysql_store_result(&my_sql);
		int rows = mysql_num_rows(res);
		int fields = mysql_num_fields(res);

		MYSQL_ROW line;
		for(int i = 0; i < rows; i++)
		{}
	}
}


