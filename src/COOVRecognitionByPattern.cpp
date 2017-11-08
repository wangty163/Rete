#include "COOVRecognitionByPattern.h"

#include "StringOp.h"
#include "PublicDefine.h"
#include "Rete/Structure/Condition.h"

unordered_map<string, vector<string>> COOVRecognitionByPattern::mapWord2Class;	//词到词类的索引 
Net COOVRecognitionByPattern::net;

template<class T>
void moveAllToFront(std::vector<T>& vt, std::function<bool(const T&)> needMove) {
	size_t index = 0;
	for (size_t i = 0; i < vt.size(); ++i)
		if (needMove(vt.at(i)))
			std::swap(vt.at(i), vt.at(index++));
}

COOVRecognitionByPattern::COOVRecognitionByPattern(void)
{
}

COOVRecognitionByPattern::~COOVRecognitionByPattern(void)
{
}

void COOVRecognitionByPattern::generate(std::vector<std::vector<Condition>>& result, size_t index, std::vector<Condition>& partResult, const std::vector<std::vector<std::vector<Condition>>>& cs) {
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


std::pair<std::vector<ConditionVector>, ConditionVector> COOVRecognitionByPattern::parse(const std::string& rawLine) {
	std::string line = rawLine;
	if (line.empty() || CStringOp::startsWith(line, "//"))
		return{};
	line = CStringOp::replaceAll(line, " ", "");
	line = CStringOp::replaceAll(line, "\t", "");
	std::vector<std::string> parts = CStringOp::split(line, "->");
	parts.push_back("");

	std::string pattern = parts.at(0), rhs = parts.at(1);

	//const std::string& pattern, const std::string& rhs;
	std::unordered_map<std::string, std::unordered_set<std::string>> dict;
	auto&& vec = CStringOp::getSequence(pattern);
	int status = 0;
	// { { {a} } , { {b} , {c}, {d, e} } }     ->     a & ( b | c | (d & e) )
	std::vector<std::vector<std::vector<Condition>>> conditions;
	std::vector<std::vector<Condition>> tmpConditions;
	std::vector<std::string> memory;
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
					auto&& tmpParam = getTmpParam();
					ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
					ret.emplace_back(tmpParam, memory.at(3) + "_" + memory.at(5), memory.at(4));
				}
				else if (memory.at(3) == "prefix") {
					auto&& tmpParam = getTmpParam();
					ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
					ret.emplace_back(tmpParam, memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "suffix") {
					auto&& tmpParam = getTmpParam();
					ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
					ret.emplace_back(tmpParam, memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "antonym") {
					auto&& tmpParam = getTmpParam();
					ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
					ret.emplace_back(tmpParam, memory.at(3), memory.at(4));
				}
				else if (memory.at(3) == "synonym") {
					auto&& tmpParam = getTmpParam();
					ret.emplace_back(memory.at(0), memory.at(1), tmpParam);
					ret.emplace_back(tmpParam, memory.at(3), memory.at(4));
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
		auto&& paramString = Field::getParamString("w", blockCount);
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
		//debugII(status, input);
		switch (status)
		{
		case 0:
			if (input == "[") {
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
		//debugI(status);
	}
	myAssertPlus(status == 24, pattern);

	std::vector<std::vector<Condition>> result;
	generate(result, 0, std::vector<Condition>(), conditions);

	std::vector<Condition> paramGetter;
	if (rhs.empty()) {
		paramGetter.emplace_back(rawLine, "oov", "");
		paramGetter.reserve(blockCount + 1);
		for (size_t i = 0; i < blockCount; ++i)
			paramGetter.emplace_back(Field::getParamString("w", i), "", "");
	}
	else {
		size_t i = CStringOp::Find(rhs, "(");
		myAssertPlus(i != std::string::npos, rhs);
		myAssertPlus(rhs.back() == ')', rhs);
		auto&& opType = rhs.substr(0, i);
		auto&& params = CStringOp::split(rhs.substr(i + 1, rhs.length() - i - 2), ",");
		paramGetter.emplace_back(rawLine, opType, "");
		if (opType == "ok" || opType == "mark") {
			myAssertPlus(params.size() == 1, rhs);
			myAssertPlus(dict[params.front()].size() == 1, pattern + "->" + rhs);
			paramGetter.emplace_back(*(dict[params.front()].begin()), "", "");
		}
		else if (opType == "rewrite") {
			myAssertPlus(params.size() == 2, rhs);
			myAssertPlus(dict[params.front()].size() == 1, pattern + "->" + rhs);
			paramGetter.emplace_back(*(dict[params.front()].begin()), params.at(1), "");
		}
		else
			myAssertPlus(false, rhs);
	}

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

	return std::make_pair(result, paramGetter);
}

void COOVRecognitionByPattern::addReteJudgeFunctions() {
	net.addFunction("numeric_greater_than", [](const std::string& left, const std::string& right) {
		double leftValue = CStringOp::FromString<double>(left);
		double rightValue = CStringOp::FromString<double>(right);
		return leftValue > rightValue;
	});
	net.addFunction("combine_0", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("combine_1", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("combine_2", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("antonym", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("synonym", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("prefix", [](const std::string& left, const std::string& right) {
		return true;
	});
	net.addFunction("suffix", [](const std::string& left, const std::string right) {
		return true;
	});
}

void COOVRecognitionByPattern::addSentence(const std::vector<std::string>& words, const std::vector<std::string>& poses) {
	for (size_t i = 0; i < words.size(); ++i) {
		std::string name = CStringOp::ToString(i) + "-" + CStringOp::ToString(i + 1);
		auto&& word = words.at(i);

		net.addWME({ name, ".pos", poses.at(i) });
		net.addWME({ name, ".word", word });
		net.addWME({ name, ".len", CStringOp::getSequenceCnt(word) });
		net.addWME({ name, ".offset_start", i });
		net.addWME({ name, ".offset_end", i + 1 });

		auto&& dict = mapWord2Class;
		if (dict.count(word) > 0)
			for (auto&& c : dict.find(word)->second)
				net.addWME({ name, ".in", c });
	}
}

/*
*功能说明：初始化函数
*创建者：刘亮亮
*创建时间：2014-11-6
*参数说明：词典初始化，读入词类词典和规则词典
*返回值：读取成功返回true,否则返回false
*/
bool COOVRecognitionByPattern::Init()
{
	bool bFlag = true;

	addReteJudgeFunctions();

	if (!ReadWordFile())
	{
		bFlag = false;
	}

	if (!ReadPatternFile())
	{
		bFlag = false;
	}

	return bFlag;
}
/*
*功能说明：读取词类词典，建立词到词类的索引
*创建者：刘亮亮
*创建时间：2010-10-30
*参数说明：sFileName-词类词典
*返回值：读取成功返回true,否则返回false
*/
bool COOVRecognitionByPattern::ReadWordFile(const string& sFileName)
{
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
	return true;
}
/*
*功能说明：读取规则库，建立谓词到规则的索引
*创建者：刘亮亮
*创建时间：2010-10-19
*参数说明：sFileName-新词识别模式文件
*返回值：读取成功返回true,否则返回false
*/
bool COOVRecognitionByPattern::ReadPatternFile(const string& sFileName)
{
	std::ifstream fin(sFileName);
	myAssert(!fin.fail());
	std::string line;

	while (!fin.eof()) {
		getline(fin, line);
		auto&& pair = parse(line);
		for (auto&& conditions : pair.first) {
			net.addProduction(conditions, pair.second);
		}
	}
	return true;
}

/*
*功能说明：模式识别
	1）通过词、词类、词性找到候选模式集合，然后进行匹配
	2）单模式匹配成功，不再继续匹配
*创建者：刘亮亮
*创建时间：2015-3-28
*参数说明：
vecResult：返回值，成功识别的新词结果
vecWords：词语序列
vecPos：词性序列
*返回值：匹配成功返回true,否则返回false
*/
bool COOVRecognitionByPattern::RecognitionByOOVPattern(vector<MatchResultNode>&vecResult, const std::vector<std::string>& words, const std::vector<std::string>& poses) {
	vecResult.clear();

	using namespace Field;
	addSentence(words, poses);
	for (auto&& info : net.invoke()) {
		myAssert(info.size() > 1);
		auto&& matchedPattern = info.front().get(id);

		auto&& opType = info.front().get(attr);
		MatchType type;
		if (opType == "ok")
			type = ok;
		else if (opType == "mark")
			type = mark;
		else if (opType == "oov")
			type = oov;
		else if (opType == "rewrite")
			type = rewrite;
		else
			myAssertPlus(false, opType);

		std::string beginStr, endStr, correctWord;
		auto&& parts = CStringOp::split(info.at(1).get(id), "-");
		myAssertPlus(parts.size() == 2, info.at(1).get(id));
		beginStr = parts.front();
		for (size_t i = 1; i < info.size(); ++i) {
			auto&& str = info.at(i).get(id);
			auto&& parts = CStringOp::split(str, "-");
			myAssertPlus(parts.size() == 2, str);
			myAssertPlus(endStr.empty() || endStr == parts.front(), "结果在原句子中不连续");
			endStr = parts.at(1);
			correctWord += info.at(i).get(attr);
		}

		vecResult.push_back({
			CStringOp::FromString<int>(beginStr),
			CStringOp::FromString<int>(endStr) - 1,
			type,
			correctWord,
			matchedPattern
		});
	}

	for (auto&& opType : { oov, ok, mark, rewrite }) {
		auto fun = [&opType](const MatchResultNode& nd) {
			return nd.type == opType;
		};
		moveAllToFront<MatchResultNode>(vecResult, fun);
	}

	net.clearStatus();
	return !vecResult.empty();
}