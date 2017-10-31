#include <iostream>

#include "ReteNet/Net.h"
#include "TestNode/TestAtTokenFilterNode.h"
#include "../Log.h"
#include "../StringOp.h"

using namespace std;


bool isValidWordClass(const std::string& item) {
	if (!item.empty() && item.front() == '!') {
		// 词类说明符：感叹号之后都是汉字
		auto&& sequence = CStringOp::getSequence(item);
		for (size_t i = 1; i < sequence.size(); ++i)
			if (sequence.at(i).length() <= 1)
				return false;
		return true;
	}
	else {
		return false;
	}
}

bool isValidWord(const std::string& item) {
	auto&& sequence = CStringOp::getSequence(item);
	for (auto&& c : sequence) {
		if (c.length() <= 1)
			return false;
	}
	return true;
}

#include <fstream>
void test_3() {
	std::ifstream fin(R"(oov_pattern.txt)");
	myAssert(!fin.fail());
	std::string line;
	std::unordered_set<std::string> st;
	while (!fin.eof()) {
		getline(fin, line);
		if (line.empty() || CStringOp::startsWith(line, "//"))
			continue;

		// line -> {condition}, rhs
		CStringOp::replaceAll(line, " ", "");
		CStringOp::replaceAll(line, "\t", "");
		std::vector<std::string> parts = CStringOp::split(line, "->");
		myAssert(!parts.empty());
		line = parts[0];
		myAssert(line.front() == '[' && line.back() == ']');
		line.pop_back();
		line = line.substr(1);
		std::vector<std::string> conditionStrings = CStringOp::split(line, "][");
		std::string rhs = parts.size() > 1 ? parts.at(1) : "ok(ALL)";

		// replace param
		// 规则中出现的变量名 -> 真正使用的变量名
		std::unordered_map<std::string, std::string> paramDict;
		std::vector<std::vector<std::vector<Condition>>> trimedConditions;
		for (size_t i = 0; i < conditionStrings.size(); ++i) {
			auto&& block = conditionStrings.at(i);
			std::unordered_set<std::string> paramSet;
			std::vector<std::vector<Condition>> blockConditions;
			for (auto& items : CStringOp::split(block, "&")) {
				myAssertPlus(!items.empty(), line);
				std::vector<Condition> itemConditions;
				for (auto& item : CStringOp::split(items, "|")) {
					myAssertPlus(!item.empty(), items);
					auto&& standardParamName = Field::getParamString(i);
					// 词类
					if (item.front() == '!') {
						myAssertPlus(isValidWordClass(item), item);
						itemConditions.emplace_back(standardParamName, "in", item);
					}
					// 都是汉字
					else if (!isascii(item.front())) {
						myAssertPlus(isValidWord(item), item);
						itemConditions.emplace_back(standardParamName, "word", item);
					}
					// 变量名
					else if (item.front() == 'w') {
						size_t ind = 1;
						while (ind < item.size() &&
							item.at(ind) != '.' &&
							item.at(ind) != '<' &&
							item.at(ind) != '>' &&
							item.at(ind) != '!' &&
							item.at(ind) != '=') {
							ind++;
						}
						paramSet.insert(item.substr(0, ind));
						paramDict.insert_or_assign(item.substr(0, ind), standardParamName);
						myAssertPlus(paramSet.size() <= 1, block);
						// item中只含有一个变量名称
						if (ind == item.size()) {
						}
						else {
							// item中含有属性筛选符
							if (item.at(ind) == '.') {

							}
							// item中没有属性筛选符
							else {
								size_t start = ind;
								while (ind < item.size() &&
									(item.at(ind) == '>' ||
										item.at(ind) == '<' ||
										item.at(ind) == '=' ||
										item.at(ind) == '!')) {
									ind++;
								}
								std::string opType = item.substr(start, ind - start);
								st.insert(opType);
								//std::cout << item << " " << opType << std::endl;
							}
						}
					}
					// 其他符号
					else {
						myAssertPlus(!isalpha(item.front()), "【" + item + "】: 独立出现的字母，有可能是词性，也有可能是词语，规则中不允许出现");

						itemConditions.emplace_back(Field::getParamString(i), "word", item);
					}
				}
				blockConditions.push_back(itemConditions);
			}
		}

		// trim format
		std::vector<std::vector<std::vector<std::string>>> all;
		for (size_t i = 0; i < conditionStrings.size(); ++i) {
			auto&& part = conditionStrings.at(i);
			myAssertPlus(CStringOp::Find(part, "[") == std::string::npos, part);
			myAssertPlus(CStringOp::Find(part, "]") == std::string::npos, part);
			myAssertPlus(!part.empty(), part);
			std::vector<std::vector<std::string>> trimedCondition;
			if (CStringOp::Find(part, "&")) {
				auto&& andParts = CStringOp::split(part, "&");
				for (auto&& andPart : andParts) {
					if (CStringOp::Find(andPart, "|")) {
						trimedCondition.push_back({});
						auto&& orParts = CStringOp::split(part, "|");
						for (auto&& orPart : orParts) {

						}
					}
					else {
						trimedCondition.push_back({ andPart });
					}
				}
			}
			else {

			}

		}
	}
	for (auto&& t : st) {
		std::cout << t << std::endl;
	}
}

namespace CStringOp {
	bool isChinese(const std::string& str) {
		return !str.empty() && !isascii((unsigned char)str.front());
	}
	bool isDigit(const std::string& str) {
		return !str.empty() && isdigit((unsigned char)str.front());
	}
	bool isAlpha(const std::string& str) {
		return !str.empty() && isalpha((unsigned char)str.front());
	}
	bool isValidWord(const std::string& str) {
		return !str.empty() && str != "[" && str != "]" && 
			str != "|" && str != "&" && (
				str == "/" || str == "\\" || isChinese(str));
	}
}

//#define main_debug_output
#ifndef main_debug_output
#define debugLine
#define debugI(v)
#define debugII(a, b)
#define debugIII(a, b, c)
#define debugIV(a, b, c, d)
#define debugV(a, b, c, d, e)
#endif

std::vector<std::vector<std::vector<Condition>>> dfa(const std::string& pattern) {
	auto&& vec = CStringOp::getSequence(pattern);
	int status = 0;
	// { { a } , { b , c } }     ->     a & (b | c)
	std::vector<std::vector<std::vector<Condition>>> conditions;
	std::vector<std::vector<Condition>> tmpConditions;
	std::vector<std::string> memory(100);
	Condition::Type type = Condition::positive;
	// 规则中出现的变量名 -> 真正使用的变量名
	std::unordered_map<std::string, std::string> paramMapping;
	int memoryIndex = -1, blockCount = 0, tmpParamCount = 0;

	auto&& getTmpParam = [&tmpParamCount]() {
		return Field::getParamString("t", tmpParamCount++);
	};
	auto&& memoryToCondition = [&memory, &memoryIndex, &pattern, &getTmpParam]() {
		myAssertPlus(memoryIndex + 1 >= 4, pattern);
		std::vector<Condition> ret;
		if (memoryIndex + 1 == 4) {
			if (memory.at(2) == "=")
				ret.emplace_back(memory.at(0), memory.at(1), memory.at(3));
			else if (memory.at(2) == "!=")
				ret.emplace_back(memory.at(0), memory.at(1), memory.at(3), Condition::negetive);
			else if (memory.at(2) == ">") {
				auto&& tmpParam = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
				ret.emplace_back(tmpParam, "numeric_greater_than", memory.at(3));
			}
			else if (memory.at(2) == "<") {
				auto&& tmpParam = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
				ret.emplace_back(tmpParam, "numeric_less_than", memory.at(3));
			}
			else if (memory.at(2) == ">=") {
				auto&& tmpParam = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
				ret.emplace_back(tmpParam, "numeric_greater_equal_than", memory.at(3));
			}
			else if (memory.at(2) == "<=") {
				auto&& tmpParam = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
				ret.emplace_back(tmpParam, "numeric_less_equal_than", memory.at(3));
			}
			else
				myAssertPlus(false, pattern);
		}
		else {
			if (memory.at(2) == "~") {
				if (memory.at(3) == "combine") {
					ret.emplace_back(memory.at(0), memory.at(3) + "_" + memory.at(5), memory.at(4));
				}
				else if (memory.at(3) == "prefix") {
					ret.emplace_back(memory.at(0), memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "suffix") {
					ret.emplace_back(memory.at(0), memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "antonym") {
					ret.emplace_back(memory.at(0), memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "synonym") {
					ret.emplace_back(memory.at(0), memory.at(3), memory.at(4));
				}
				else
					myAssertPlus(false, pattern);
			}
			else if (memory.at(2) == "=") {
				auto&& tmpParam = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam);
			}
			else if (memory.at(2) == "!=") {
				auto&& tmpParam0 = getTmpParam();
				auto&& tmpParam1 = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam0);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam1);
				ret.emplace_back(tmpParam0, "equal", tmpParam1, Condition::negetive);
			}
			else if (memory.at(2) == ">") {
				auto&& tmpParam0 = getTmpParam();
				auto&& tmpParam1 = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam0);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam1);
				ret.emplace_back(tmpParam0, "numeric_greater_than", tmpParam1);
			}
			else if (memory.at(2) == "<") {
				auto&& tmpParam0 = getTmpParam();
				auto&& tmpParam1 = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam0);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam1);
				ret.emplace_back(tmpParam0, "numeric_less_than", tmpParam1);
			}
			else if (memory.at(2) == ">=") {
				auto&& tmpParam0 = getTmpParam();
				auto&& tmpParam1 = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam0);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam1);
				ret.emplace_back(tmpParam0, "numeric_greater_equal_than", tmpParam1);
			}
			else if (memory.at(2) == "<=") {
				auto&& tmpParam0 = getTmpParam();
				auto&& tmpParam1 = getTmpParam();
				ret.emplace_back(memory.at(0), memory.at(1), tmpParam0);
				ret.emplace_back(memory.at(3), memory.at(4), tmpParam1);
				ret.emplace_back(tmpParam0, "numeric_less_equal_than", tmpParam1);
			}
			else
				myAssertPlus(false, pattern);
		}
		return ret;
	};
	auto&& normalizeParam = [&memory, &paramMapping, &blockCount, &pattern](int index) {
		std::string& paramName = memory.at(index);
		std::string& paramString = Field::getParamString("w", blockCount);
		myAssertPlus(paramMapping.count(paramName) == 0 || 
			paramMapping[paramName] == paramString, pattern);
		paramMapping.insert({ paramName, paramString });
		paramName = paramString;
	};
	auto&& getNormalizedParam = [&memory, &paramMapping, &blockCount, &pattern](int index) {
		std::string& paramName = memory.at(index);
		myAssertPlus(paramMapping.count(paramName) != 0, pattern);
		memory.at(index) = paramMapping[paramName];
	};

	auto&& generateCondition = [&tmpConditions, &memory, &memoryIndex, &memoryToCondition]() {
		tmpConditions.push_back(memoryToCondition());
		//tmpConditions.push_back(std::vector<std::string>(memoryIndex + 1));
		//for (int i = 0; i <= memoryIndex; ++i)
		//	tmpConditions.back().at(i) = memory.at(i);
	};
	auto&& generateConditionAtOrOp = [&generateCondition]() {
		generateCondition();
	};
	auto&& generateConditionAtAndOp = [&generateCondition, &tmpConditions, &conditions]() {
		generateCondition();
		conditions.push_back(tmpConditions);
		tmpConditions.clear();
	};
	auto&& generateConditionAtBlockEnd = [&generateCondition, &tmpConditions, &conditions]() {
		generateCondition();
		conditions.push_back(tmpConditions);
		tmpConditions.clear();
	};

	for (size_t index = 0; index < vec.size() && status >= 0; ++index) {
		std::string& input = vec.at(index);
		debugII(status, input);
		switch (status)
		{
		case 0:
			if (input == "[") {
				memory.at(0) = Field::getParamString("w", blockCount);
				memory.at(1) = ".at_start";
				memory.at(2) = "=";
				memory.at(memoryIndex = 3) = "*";
				generateCondition();
				conditions.push_back(tmpConditions);
				tmpConditions.clear();
				//conditions.push_back({ {Field::getParamString("w", 0), ".at_start", "=", "*"} });
				status = 1;
			}
			else status = -1;
			break;
		case 1:
			if (input == "!") {
				memory.at(0) = Field::getParamString("w", blockCount);
				memory.at(1) = ".in";
				memory.at(2) = "=";
				memory.at(memoryIndex = 3) = input;
				status = 2;
			}
			else if (input == "w") {
				memory.at(memoryIndex = 0) = input;
				status = 25;
			}
			else if (CStringOp::isChinese(input)) {
				memory.at(0) = Field::getParamString("w", blockCount);
				memory.at(1) = ".word";
				memory.at(2) = "=";
				memory.at(memoryIndex = 3) = input;
				status = 4;
			}
			else if (input == "/" || input == "\\") {
				memory.at(0) = Field::getParamString("w", blockCount);
				memory.at(1) = ".word";
				memory.at(2) = "=";
				memory.at(memoryIndex = 3) = input;
				status = 4;
			}
			else status = -1;
			break;
		case 2:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 5;
			}
			break;
		case 3:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 3;
			}
			else if (input == "~") {
				normalizeParam(0);
				memory.at(1) = ".word";
				memory.at(memoryIndex = 2) = "~";
				status = 6;
			}
			else if (input == ".") {
				normalizeParam(0);
				memory.at(memoryIndex = 1) = input;
				status = 7;
			}
			else if (input == "!") {
				normalizeParam(0);
				memory.at(1) = ".word";
				memory.at(memoryIndex = 2) = input;
				status = 8;
			}
			else if (input == "=") {
				normalizeParam(0);
				memory.at(1) = ".word";
				memory.at(memoryIndex = 2) = input;
				status = 9;
			}
			else if (input == "]") {
				normalizeParam(0);
				status = 24;
			}
			else status = -1;
			break;
		case 4:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 4;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 5:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 5;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 6:
			if (CStringOp::isAlpha(input)) {
				memory.at(++memoryIndex) = input;
				status = 10;
			}
			else status = -1;
			break;
		case 7:
			if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 11;
			}
			else status = -1;
			break;
		case 8:
			if (input == "=") {
				memory.at(memoryIndex) += input;
				status = 9;
			}
			else status = -1;
			break;
		case 9:
			if (input == "!") {
				memory.at(++memoryIndex) = input;
				status = 16;
			}
			else if (CStringOp::isDigit(input)) {
				memory.at(++memoryIndex) = input;
				status = 17;
			}
			else if (input == "w") {
				memory.at(++memoryIndex) = input;
				status = 18;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.at(++memoryIndex) = input;
				status = 19;
			}
			else if (CStringOp::isChinese(input)) {
				memory.at(++memoryIndex) = input;
				status = 20;
			}
			else status = -1;
			break;
		case 10:
			if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 10;
			}
			else if (input == "(") {
				memory.at(++memoryIndex) = "";
				status = 12;
			}
			else status = -1;
			break;
		case 11:
			if (input == "~") {
				memory.at(++memoryIndex) = input;
				status = 6;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 11;
			}
			else if (input == "=") {
				memory.at(++memoryIndex) = input;
				status = 9;
			}
			else if (input == ">") {
				memory.at(++memoryIndex) = input;
				status = 13;
			}
			else if (input == "<") {
				memory.at(++memoryIndex) = input;
				status = 13;
			}
			else status = -1;
			break;
		case 12:
			if (input == "w") {
				memory.at(memoryIndex) = input;
				status = 14;
			}
			else if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) = input;
				status = 26;
			}
			else if (CStringOp::isDigit(input) || CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) = input;
				status = 26;
			}
			else status = -1;
			break;
		case 13:
			if (input == "=") {
				memory.at(memoryIndex) += input;
				status = 9;
			}
			else if (input == "!") {
				memory.at(++memoryIndex) = input;
				status = 16;
			}
			else if (CStringOp::isDigit(input)) {
				memory.at(++memoryIndex) = input;
				status = 17;
			}
			else if (input == "w") {
				memory.at(++memoryIndex) = input;
				status = 18;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.at(++memoryIndex) = input;
				status = 19;
			}
			else if (CStringOp::isChinese(input)) {
				memory.at(++memoryIndex) = input;
				status = 20;
			}
			else status = -1;
			break;
		case 14:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 27;
			}
			else if (input == ")") {
				status = 15;
			}
			else if (input == ",") {
				memory.at(++memoryIndex) = "";
				status = 12;
			}
			else status = -1;
			break;
		case 15:
			if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 16:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 21;
			}
			else status = -1;
			break;
		case 17:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 17;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 18:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 28;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 19:
			if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 19;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 20:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 20;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 21:
			if (CStringOp::isChinese(input)) {
				memory.at(memoryIndex) += input;
				status = 21;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 22:
			if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 23;
			}
			else status = -1;
			break;
		case 23:
			if (CStringOp::isAlpha(input)) {
				memory.at(memoryIndex) += input;
				status = 23;
			}
			else if (input == "|") {
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 24:
			if (input == "[") status = 1;
			else status = -1;
			break;
		case 25:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 3;
			}
			else status = -1;
			break;
		case 26:
			if (CStringOp::isChinese(input) || CStringOp::isAlpha(input) ||
				CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 26;
			}
			else if (input == ")") {
				status = 15;
			}
			else if (input == ",") {
				memory.at(++memoryIndex) = "";
				status = 12;
			}
			else status = -1;
			break;
		case 27:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 27;
			}
			else if (input == ")") {
				getNormalizedParam(memoryIndex);
				status = 15;
			}
			else if (input == ",") {
				getNormalizedParam(memoryIndex);
				memory.at(++memoryIndex) = "";
				status = 12;
			}
			else status = -1;
			break;
		case 28:
			if (CStringOp::isDigit(input)) {
				memory.at(memoryIndex) += input;
				status = 28;
			}
			else if (input == ".") {
				getNormalizedParam(memoryIndex);
				memory.at(++memoryIndex) = input;
				status = 22;
			}
			else if (input == "|") {
				getNormalizedParam(memoryIndex);
				memory.at(++memoryIndex) = ".word";
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				getNormalizedParam(memoryIndex);
				memory.at(++memoryIndex) = ".word";
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				getNormalizedParam(memoryIndex);
				memory.at(++memoryIndex) = ".word";
				generateConditionAtBlockEnd();
				status = 24;
			}
			break;
		}
		if (input == "]") {
			++blockCount;
		}
		else if (input == "[") {
			if (blockCount != 0) {
				memory.at(0) = Field::getParamString("w", blockCount - 1);
				memory.at(1) = ".offset_end";
				memory.at(2) = "=";
				memory.at(3) = Field::getParamString("w", blockCount);
				memory.at(memoryIndex = 4) = ".offset_start";
				generateCondition();
				conditions.push_back(tmpConditions);
				tmpConditions.clear();
				//conditions.push_back({ {Field::getParamString("w", blockCount - 1), ".offset_end", "=", Field::getParamString("w", blockCount), ".offset_start"} });
			}
		}
		debugI(status);
	}
	memory.at(0) = Field::getParamString("w", blockCount - 1);
	memory.at(1) = ".at_end";
	memory.at(2) = "=";
	memory.at(memoryIndex = 3) = "*";
	generateCondition();
	conditions.push_back(tmpConditions);
	tmpConditions.clear();
	//conditions.push_back({ {Field::getParamString("w", blockCount - 1), ".at_end", "*"} });

#ifdef main_debug_output
	for (auto&& conditionPart : conditions) {
		puts("{");
		for (auto&& condition : conditionPart) {
			puts("\t{");
			for (auto&& str : condition) {
				str.print(2);
				puts("\t\t&");
				//std::cout << str << " ";
			}
			puts("\t}");
			puts("\t|");
		}
		puts("}");
		puts("&");
	}
#endif // main_debug_output

	myAssertPlus(status == 24, pattern);
	return conditions;
}

void generate(std::vector<std::vector<Condition>>& result, size_t index, std::vector<Condition>& partResult, const std::vector<std::vector<std::vector<Condition>>>& cs) {
	if (index == cs.size()) {
		result.push_back(partResult);
	}
	else {
		for (auto&& conditionsWithAnd : cs.at(index)) {
			for (auto&& c : conditionsWithAnd)
				partResult.push_back(c);
			generate(result, index + 1, partResult, cs);
			partResult.erase(partResult.end() - conditionsWithAnd.size(), partResult.end());
		}
	}
}

Net getNet() {
	Net net;
	net.addFunction("numeric_greater_than", [](const std::string& left, const std::string& right) {
		double leftValue = CStringOp::FromString<double>(left);
		double rightValue = CStringOp::FromString<double>(right);
		return leftValue > rightValue;
	});
	net.addFunction("combine_0", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("combine_1", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("combine_2", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("antonym", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("synonym", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("prefix", [](const std::string& left, const std::string& right) {
		return left == right;
	});
	net.addFunction("suffix", [](const std::string& l, const std::string right) {
		return true;
	});
	return net;
}

std::unordered_map<std::string, std::vector<std::string>> readWordFile(const string& sFileName=R"(C:\Users\wuyuming\OneDrive - Office\Project\ChineseProofread\rul\oov_wordclass.txt)") {
	std::unordered_map<std::string, std::vector<std::string>> mapWord2Class;
	ifstream fin(sFileName);
	std::string line;
	while (getline(fin, line)) {
		if (0 == CStringOp::Find(line, "//") || "" == line) {
			continue;
		}
		auto&& vecParts = CStringOp::split(line, "=");
		if (2 == vecParts.size()) {
			auto&& sWordClass = vecParts[0];
			//建立词条到词类的索引
			auto&& vecWords = CStringOp::split(vecParts[1], "|");
			for (size_t ix = 0; ix<vecWords.size(); ++ix) {
				mapWord2Class[vecWords.at(ix)].push_back(sWordClass);
			}
		}
	}
	return mapWord2Class;
}
void addSentence(Net& net, std::vector<std::string> wordVector, 
	std::vector<std::string> posVector) {
	myAssert(wordVector.size() == posVector.size());

	std::unordered_set<size_t> startIndex, endIndex;
	for (size_t i = 0, offset = 0; i < wordVector.size(); ++i) {
		size_t len = CStringOp::getSequenceCnt(wordVector.at(i));
		std::string name = CStringOp::ToString(offset) + "-" + CStringOp::ToString(len);

		net.addWME({ name, ".pos", posVector.at(i) });
		startIndex.insert(offset);
		endIndex.insert(offset + len);

		offset += len;
	}

	std::vector<std::string> wordCharVector;
	std::string sentence;
	for (auto&& word : wordVector)
		sentence += word;
	wordCharVector = CStringOp::getSequence(sentence);

	auto&& dict = readWordFile();
	for (size_t l = 0; l < wordCharVector.size(); ++l) {
		std::string word, ls = CStringOp::ToString(l);
		for (size_t r = l + 1; r <= min(wordCharVector.size(), l + 5); ++r) {
			word += wordCharVector.at(r - 1);
			std::string name = ls + "-" + CStringOp::ToString(r);
			net.addWME({ name, ".word", word });
			net.addWME({ name, ".len", r - l });
			net.addWME({ name, ".offset_start", l });
			net.addWME({ name, ".offset_end", r });
			if (startIndex.count(l) > 0)
				net.addWME({ name, ".at_start", "true" });
			if (endIndex.count(r) > 0)
				net.addWME({ name, ".at_end", "true" });
			if (dict.count(word) > 0)
				for (auto&& c : dict.find(word)->second)
					net.addWME({ name, ".in", c });
		}
	}
}
void test_4() {
#ifdef main_debug_output
	std::string pattern;
	while (std::cin >> pattern) {
		std::vector<std::vector<Condition>> result;
		generate(result, 0, std::vector<Condition>(), dfa(pattern));
		for (auto&& conditions : result) {
			for (auto&& c : conditions)
				c.print(0);
			puts("----------------------------------");
		}
	}
#endif // main_debug_output

	Net net = getNet();
	std::ifstream fin(R"(oov_pattern.txt)");
	myAssert(!fin.fail());
	std::string line;
	
	int cnt = 0;
	CLog::write("build network", "start");
	while (!fin.eof()) {
		getline(fin, line);
		if (line.empty() || CStringOp::startsWith(line, "//"))
			continue;
		CStringOp::replaceAll(line, " ", "");
		CStringOp::replaceAll(line, "\t", "");
		std::vector<std::string> parts = CStringOp::split(line, "->");
		std::vector<std::vector<Condition>> result;
		generate(result, 0, std::vector<Condition>(), dfa(parts.front()));
		for (auto&& conditions : result) {
			net.addProduction(conditions, parts.front());
		}
	}
	CLog::write("build network", "end");

	addSentence(net, {
		"在", "一", "次", "抗日", "战役", "中", "，", "一", "向", "跋扈", "，", "从未", "遇到", "败绩", "的", "日军", "，", "竟然", "在", "白崇禧", "那里", "系数", "被", "灭", "，", "伤亡", "率", "超国军", "两", "倍", "。"
	}, {
		"p", "m", "q", "vn", "n", "f", "w", "m", "p", "a", "w", "d", "v", "n", "uj", "n", "w", "d", "p", "nr", "r", "n", "p", "v", "w", "v", "v", "nr", "m", "q", "w"
	});

	CLog::write("invoke", "start");
	net.invoke();
	CLog::write("invoke", "end");
}

int main() {
	test_4();
	return 0;
}
