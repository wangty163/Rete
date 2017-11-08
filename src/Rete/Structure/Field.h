#pragma once

#include <string>

#include "../../StringOp.h"

namespace Field
{
	enum Name {
		id,
		attr,
		value,
		undefined,
	};
	extern char prefixChar;

	template<typename T>
	std::string getParamString(const T& paramName);
	std::string getParamString(const std::string& prefix, size_t paramCount);
	std::string getParamName(const std::string& str);
	bool isParam(const std::string& str);
};


template<typename T>
inline std::string Field::getParamString(const T & paramName) {
	return prefixChar + CStringOp::ToString(paramName);
}
