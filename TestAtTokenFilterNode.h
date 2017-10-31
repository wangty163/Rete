#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include "Condition.h"

class TestAtTokenFilterNode {
public:
	using JudgeFunctionType = std::function<bool(const std::string&, const std::string&)>;
	bool performTest(const Condition& c);
	bool isNeedFilterToken(const Condition& c);
	void insertJudgeFunction(const std::string& key, JudgeFunctionType& value);
private:
	std::unordered_map<std::string, JudgeFunctionType> dict;
};