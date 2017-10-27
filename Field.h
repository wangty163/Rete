#pragma once

#include <string>

namespace Field
{
	enum Name {
		id,
		attr,
		value,
		undefined,
	};
	extern char prefixChar;

	std::string getParamString(const std::string& paramName);
	std::string getParamName(const std::string& str);
	bool isParam(const std::string& str);
};

