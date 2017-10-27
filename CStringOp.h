#pragma once

#include <string>
#include <sstream>

namespace CStringOp {
	size_t find(const std::string& str, const std::string& sub, size_t offset = 0);
	std::string repeat(const std::string & str, size_t repeatCnt);
	template <class CT> CT FromString(const std::string& str);
	template <class CT> std::string ToString(CT t);
}

template <class CT> CT CStringOp::FromString(const std::string& str)
{
	CT t;
	std::istringstream istr(str);
	istr >> t;
	return t;
}
template <class CT>
std::string CStringOp::ToString(CT t)
{
	std::ostringstream ostr;
	ostr << t;
	//ostr << setprecision(8) << fixed << t;  //固定不以科学计数法输出，用cout.unsetf(ostream::floatfield)取消
	return ostr.str();
}