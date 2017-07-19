#include"sql.h"

void select_data(char* buf)
{

}
int main()
{
	sql obj("root", "", "127.0.0.1", "class", 3306);
	obj.connect();
	obj.select();
	return 0;
}
