#pragma once

#include <unordered_map>
#include <string>

#include "Condition.h"

namespace TestAtTokenFilterNode {
	using JudgeFunctionType = bool(*)(const std::string&, const std::string&);

	extern const std::unordered_map<std::string, JudgeFunctionType> dict;
	
	bool performTest(const Condition& c);
	bool isNeedFilterToken(const Condition& c);
};