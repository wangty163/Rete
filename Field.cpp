#include "Field.h"

#include "PublicDefine.h"

char Field::prefixChar = '$';

std::string Field::getParamString(const std::string & prefix, size_t paramCount) {
	return prefixChar + prefix + CStringOp::ToString(paramCount);
}

std::string Field::getParamName(const std::string & str) {
	myAssert(!str.empty() && str.front() == prefixChar);
	return str.substr(1);
}

bool Field::isParam(const std::string & str) {
	return !str.empty() && str.front() == prefixChar;
}
