#include "CStringOp.h"

std::string CStringOp::repeat(const std::string & str, size_t repeatCnt) {
	std::string ret;
	for (size_t i = 0; i < repeatCnt; ++i)
		ret += str;
	return ret;
}

size_t CStringOp::find(const std::string& str, const std::string& sub, size_t offset)
{
	size_t nSize1 = str.size();
	size_t nSize2 = sub.size();

	if (nSize1 == 0 || nSize2 == 0 || nSize1<nSize2)
		return std::string::npos;

	size_t k = offset;
	size_t d = nSize1 - nSize2;

	while (k <= d)
	{
		if (sub == str.substr(k, nSize2))
			return k;

		if (!isascii(str[k]))
			//if (str[k] < 0)
			k += 2;
		else
			++k;
	}

	return std::string::npos;
}