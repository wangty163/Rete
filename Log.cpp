#include "Log.h"

string Now() {
	time_t t = time(0);
	char tmp[32];

	struct tm tt;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&tt, &now);   //获取当地日期和时间

	strftime(tmp, sizeof(tmp), "[%Y-%m-%d %X]", &tt);
	return string(tmp);
}
void CLog::write(const std::string& type, const std::string & infos)
{
	std::cout << Now() << " 【" << type << "】：" << infos << std::endl;
}

void CLog::write(const std::string & infos)
{
	std::cout << Now() << infos << std::endl;
}

