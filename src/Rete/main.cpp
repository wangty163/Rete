#include <iostream>

#include "ReteNet/Net.h"
#include "TestNode/TestAtTokenFilterNode.h"
#include "../Log.h"
#include "../StringOp.h"
#include "Utils/VectorPrinter.h"

//#define main_debug_output
#ifndef main_debug_output
#define debugLine
#define debugI(v)
#define debugII(a, b)
#define debugIII(a, b, c)
#define debugIV(a, b, c, d)
#define debugV(a, b, c, d, e)
#endif

using namespace std;

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

std::pair<std::vector<std::vector<Condition>>, std::vector<Condition>> parse(const std::string& pattern, const std::string& rhs) {
	std::unordered_map<std::string, std::unordered_set<std::string>> dict;
	auto&& vec = CStringOp::getSequence(pattern);
	int status = 0;
	// { { a } , { b , c } }     ->     a & (b | c)
	std::vector<std::vector<std::vector<Condition>>> conditions;
	std::vector<std::vector<Condition>> tmpConditions;
	std::vector<std::string> memory;
	Condition::Type type = Condition::positive;
	// 规则中出现的变量名 -> 真正使用的变量名
	std::unordered_map<std::string, std::string> paramMapping;
	int blockCount = 0, tmpParamCount = 0;

	auto&& getTmpParam = [&tmpParamCount]() {
		return Field::getParamString("t", tmpParamCount++);
	};
	auto&& memoryToCondition = [&memory, &pattern, &getTmpParam]() {
		myAssertPlus(memory.size() >= 4, pattern);
		std::vector<Condition> ret;
		if (memory.size() == 4) {
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
		memory.clear();
		return ret;
	};
	auto&& normalizeParam = [&memory, &paramMapping, &blockCount, &pattern]() {
		std::string& paramName = memory.back();
		std::string& paramString = Field::getParamString("w", blockCount);
		myAssertPlus(paramMapping.count(paramName) == 0 || 
			paramMapping[paramName] == paramString, pattern);
		paramMapping.insert({ paramName, paramString });
		paramName = paramString;
	};
	auto&& getNormalizedParam = [&memory, &paramMapping, &blockCount, &pattern]() {
		std::string& paramName = memory.back();
		myAssertPlus(paramMapping.count(paramName) != 0, pattern);
		memory.back() = paramMapping[paramName];
	};
	auto&& generateTmpCondition = [&tmpConditions, &memoryToCondition]() {
		tmpConditions.push_back(memoryToCondition());
	};
	auto&& generateCondition = [&tmpConditions, &conditions]() {
		conditions.push_back(tmpConditions);
		tmpConditions.clear();
	};
	auto&& generateConditionAtOrOp = [&generateTmpCondition]() {
		generateTmpCondition();
	};
	auto&& generateConditionAtAndOp = [&generateTmpCondition, &generateCondition, &tmpConditions, &conditions]() {
		generateTmpCondition();
		generateCondition();
	};
	auto&& generateConditionAtBlockEnd = [&generateTmpCondition, &generateCondition, &tmpConditions, &conditions]() {
		generateTmpCondition();
		generateCondition();
	};

	for (size_t index = 0; index < vec.size() && status >= 0; ++index) {
		std::string& input = vec.at(index);
		debugII(status, input);
		switch (status)
		{
		case 0:
			if (input == "[") {
				memory.push_back(Field::getParamString("w", blockCount));
				memory.push_back(".at_start");
				memory.push_back("=");
				memory.push_back("*");
				generateTmpCondition();
				generateCondition();
				status = 1;
			}
			else status = -1;
			break;
		case 1:
			if (input == "!") {
				memory.push_back(Field::getParamString("w", blockCount));
				memory.push_back(".in");
				memory.push_back("=");
				memory.push_back(input);
				status = 2;
			}
			else if (input == "w") {
				memory.push_back(input);
				status = 25;
			}
			else if (CStringOp::isChinese(input)) {
				memory.push_back(Field::getParamString("w", blockCount));
				memory.push_back(".word");
				memory.push_back("=");
				memory.push_back(input);
				status = 4;
			}
			else if (input == "/" || input == "\\") {
				memory.push_back(Field::getParamString("w", blockCount));
				memory.push_back(".word");
				memory.push_back("=");
				memory.push_back(input);
				status = 4;
			}
			else status = -1;
			break;
		case 2:
			if (CStringOp::isChinese(input)) {
				memory.back() += input;
				status = 5;
			}
			break;
		case 3:
			if (CStringOp::isDigit(input)) {
				memory.back() += input;
				status = 3;
			}
			else if (input == "~") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				normalizeParam();
				memory.push_back(".word");
				memory.push_back("~");
				status = 6;
			}
			else if (input == ".") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				normalizeParam();
				memory.push_back(input);
				status = 7;
			}
			else if (input == "!") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				normalizeParam();
				memory.push_back(".word");
				memory.push_back(input);
				status = 8;
			}
			else if (input == "=") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				normalizeParam();
				memory.push_back(".word");
				memory.push_back(input);
				status = 9;
			}
			else if (input == "]") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				normalizeParam();
				memory.clear();
				status = 24;
			}
			else status = -1;
			break;
		case 4:
			if (CStringOp::isChinese(input)) {
				memory.back() += input;
				status = 4;
			}
			else if (input == "|") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 5:
			if (CStringOp::isChinese(input)) {
				memory.back() += input;
				status = 5;
			}
			else if (input == "|") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				dict[memory.back()].insert(Field::getParamString("w", blockCount));
				generateConditionAtBlockEnd();
				status = 24;
			}
			else status = -1;
			break;
		case 6:
			if (CStringOp::isAlpha(input)) {
				memory.push_back(input);
				status = 10;
			}
			else status = -1;
			break;
		case 7:
			if (CStringOp::isAlpha(input)) {
				memory.back() += input;
				status = 11;
			}
			else status = -1;
			break;
		case 8:
			if (input == "=") {
				memory.back() += input;
				status = 9;
			}
			else status = -1;
			break;
		case 9:
			if (input == "!") {
				memory.push_back(input);
				status = 16;
			}
			else if (CStringOp::isDigit(input)) {
				memory.push_back(input);
				status = 17;
			}
			else if (input == "w") {
				memory.push_back(input);
				status = 18;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.push_back(input);
				status = 19;
			}
			else if (CStringOp::isChinese(input)) {
				memory.push_back(input);
				status = 20;
			}
			else status = -1;
			break;
		case 10:
			if (CStringOp::isAlpha(input)) {
				memory.back() += input;
				status = 10;
			}
			else if (input == "(") {
				memory.push_back("");
				status = 12;
			}
			else status = -1;
			break;
		case 11:
			if (input == "~") {
				memory.push_back(input);
				status = 6;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.back() += input;
				status = 11;
			}
			else if (input == "=") {
				memory.push_back(input);
				status = 9;
			}
			else if (input == ">") {
				memory.push_back(input);
				status = 13;
			}
			else if (input == "<") {
				memory.push_back(input);
				status = 13;
			}
			else status = -1;
			break;
		case 12:
			if (input == "w") {
				memory.back() += input;
				status = 14;
			}
			else if (CStringOp::isChinese(input)) {
				memory.back() += input;
				status = 26;
			}
			else if (CStringOp::isDigit(input) || CStringOp::isAlpha(input)) {
				memory.back() += input;
				status = 26;
			}
			else status = -1;
			break;
		case 13:
			if (input == "=") {
				memory.back() += input;
				status = 9;
			}
			else if (input == "!") {
				memory.push_back(input);
				status = 16;
			}
			else if (CStringOp::isDigit(input)) {
				memory.push_back(input);
				status = 17;
			}
			else if (input == "w") {
				memory.push_back(input);
				status = 18;
			}
			else if (CStringOp::isAlpha(input)) {
				memory.push_back(input);
				status = 19;
			}
			else if (CStringOp::isChinese(input)) {
				memory.push_back(input);
				status = 20;
			}
			else status = -1;
			break;
		case 14:
			if (CStringOp::isDigit(input)) {
				memory.back() += input;
				status = 27;
			}
			else if (input == ")") {
				status = 15;
			}
			else if (input == ",") {
				memory.push_back("");
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
				memory.back() += input;
				status = 21;
			}
			else status = -1;
			break;
		case 17:
			if (CStringOp::isDigit(input)) {
				memory.back() += input;
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
				memory.back() += input;
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
				memory.back() += input;
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
				memory.back() += input;
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
				memory.back() += input;
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
				memory.back() += input;
				status = 23;
			}
			else status = -1;
			break;
		case 23:
			if (CStringOp::isAlpha(input)) {
				memory.back() += input;
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
				memory.back() += input;
				status = 3;
			}
			else status = -1;
			break;
		case 26:
			if (CStringOp::isChinese(input) || CStringOp::isAlpha(input) ||
				CStringOp::isDigit(input)) {
				memory.back() += input;
				status = 26;
			}
			else if (input == ")") {
				status = 15;
			}
			else if (input == ",") {
				memory.push_back("");
				status = 12;
			}
			else status = -1;
			break;
		case 27:
			if (CStringOp::isDigit(input)) {
				memory.back() += input;
				status = 27;
			}
			else if (input == ")") {
				getNormalizedParam();
				status = 15;
			}
			else if (input == ",") {
				getNormalizedParam();
				memory.push_back("");
				status = 12;
			}
			else status = -1;
			break;
		case 28:
			if (CStringOp::isDigit(input)) {
				memory.back() += input;
				status = 28;
			}
			else if (input == ".") {
				getNormalizedParam();
				memory.push_back(input);
				status = 22;
			}
			else if (input == "|") {
				getNormalizedParam();
				memory.push_back(".word");
				generateConditionAtOrOp();
				status = 1;
			}
			else if (input == "&") {
				getNormalizedParam();
				memory.push_back(".word");
				generateConditionAtAndOp();
				status = 1;
			}
			else if (input == "]") {
				getNormalizedParam();
				memory.push_back(".word");
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
				memory.push_back(Field::getParamString("w", blockCount - 1));
				memory.push_back(".offset_end");
				memory.push_back("=");
				memory.push_back(Field::getParamString("w", blockCount));
				memory.push_back(".offset_start");
				generateTmpCondition();
				generateCondition();
			}
		}
		debugI(status);
	}

	memory.push_back(Field::getParamString("w", blockCount - 1));
	memory.push_back(".at_end");
	memory.push_back("=");
	memory.push_back("*");
	generateTmpCondition();
	generateCondition();

	std::vector<Condition> paramGetter;
	if (rhs.empty()) {
		paramGetter.reserve(blockCount);
		for (size_t i = 0; i < blockCount; ++i)
			paramGetter.emplace_back(Field::getParamString("w", i), "ok", "");
	}
	else {
		size_t i = CStringOp::Find(rhs, "(");
		myAssertPlus(i != std::string::npos, rhs);
		myAssertPlus(rhs.back() == ')', rhs);
		auto&& opType = rhs.substr(0, i);
		auto&& params = CStringOp::split(rhs.substr(i + 1, rhs.length() - i - 2), ",");
		if (opType == "ok" || opType == "mark") {
			myAssertPlus(params.size() == 1, rhs);
			myAssertPlus(dict[params.front()].size() == 1, pattern + "->" + rhs);
			paramGetter.emplace_back(*(dict[params.front()].begin()), opType, "");
		}
		else if (opType == "rewrite") {
			myAssertPlus(params.size() == 2, rhs);
			myAssertPlus(dict[params.front()].size() == 1, pattern + "->" + rhs);
			paramGetter.emplace_back(*(dict[params.front()].begin()), opType, params.at(1));
		}
		else
			myAssertPlus(false, rhs);
	}
	paramGetter.emplace_back(pattern, rhs, "");
	myAssertPlus(status == 24, pattern);

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

	std::vector<std::vector<Condition>> result;
	generate(result, 0, std::vector<Condition>(), conditions);
	return std::make_pair(result, paramGetter);
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
void addSentence(Net& net, std::vector<std::string> wordVector, std::vector<std::string> posVector) {
	myAssert(wordVector.size() == posVector.size());

	std::unordered_set<size_t> startIndex, endIndex;
	for (size_t i = 0, offset = 0; i < wordVector.size(); ++i) {
		size_t len = CStringOp::getSequenceCnt(wordVector.at(i));
		std::string name = CStringOp::ToString(offset) + "-" + CStringOp::ToString(offset + len);

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
		parts.push_back("");

		auto&& pair = parse(parts.at(0), parts.at(1));

		for (auto&& conditions : pair.first) {
			net.addProduction(conditions, pair.second);
		}
	}
	CLog::write("build network", "end");

	std::vector<std::string> wordVector = {
		"毛泽东", "可谓", "是", "奋斗", "了", "一", "生", "，", "子女", "们", "却", "死", "得", "死", "，", "丢", "的", "丢", " ，", "晚年", "的", "毛泽东", "可谓", "是", "十分", "孤独", "，", "为", "大家", "创造", "了", "新", "生活", "，", "自己", "却", "不", "能", "享受", "天伦之乐", "。"
	}, posVector = {
		"nr", "v", "v", "v", "ul", "m", "v", "w", "n", "k", "d", "v", "ud", "a", "w", "v", "uj", "v", "w", "t", "uj", "nr", "v", "v", "d", "a", "w", "p", "r", "v", "ul", "a", "vn", "w", "r", "d", "d", "v", "v", "i", "w"
	};
	std::string sentence;
	for (auto&& w : wordVector)
		sentence += w;
	auto&& vec = CStringOp::getSequence(sentence);
	addSentence(net, wordVector, posVector);

	CLog::write("invoke", "start");
	auto&& infos = net.invoke();
	for (auto&& info : infos) {
		for (size_t i = 0; i + 1 < info.size(); ++i) {
			auto&& range = info.at(i).get(Field::id);
			auto&& parts = CStringOp::split(range, "-");
			size_t start = CStringOp::FromString<size_t>(parts.at(0));
			size_t end = CStringOp::FromString<size_t>(parts.at(1));
			std::string str;
			for (size_t i = start; i < end; ++i)
				str += vec.at(i);
			info.at(i).set(Field::id, str);
		}
		VectorPrinter::print(info, 0);
	}
	CLog::write("invoke", "end");
}

void test_for_debug() {
	Net net = getNet();

	// input
	std::string pattern = "[w1.pos=d][w2.pos=v][w3.pos=ud]";
	auto&& it = parse(pattern, "");
	for (auto&& conditions : it.first) {
		net.addProduction(conditions, it.second);
		for (auto&& c : conditions)
			c.print(0);
		puts("----------------------------------");
	}

	addSentence(net, {
		"毛泽东", "可谓", "是", "奋斗", "了", "一", "生", "，", "子女", "们", "却", "死", "得", "死", "，", "丢", "的", "丢", " ，", "晚年", "的", "毛泽东", "可谓", "是", "十分", "孤独", "，", "为", "大家", "创造", "了", "新", "生活", "，", "自己", "却", "不", "能", "享受", "天伦之乐", "。"
	}, {
		"nr", "v", "v", "v", "ul", "m", "v", "w", "n", "k", "d", "v", "ud", "a", "w", "v", "uj", "v", "w", "t", "uj", "nr", "v", "v", "d", "a", "w", "p", "r", "v", "ul", "a", "vn", "w", "r", "d", "d", "v", "v", "i", "w"
	});

	CLog::write("invoke", "start");
	auto&& infos = net.invoke();
	for (auto&& info : infos) {
		VectorPrinter::print(info, 0);
	}
	CLog::write("invoke", "end");
	system("pause");
}

int main() {
#ifndef main_debug_output
	test_4();
#else
	test_for_debug();
#endif
	return 0;
}
