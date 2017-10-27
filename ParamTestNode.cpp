#include "ParamTestNode.h"

#include "PublicDefine.h"

ParamTestNode::ParamTestNode(Field::Name fieldOfArg1
	, size_t conditionNumberOfArg2, Field::Name fieldOfArg2)
	: fieldOfArg1(fieldOfArg1), conditionNumberOfArg2(conditionNumberOfArg2)
	, fieldOfArg2(fieldOfArg2) {
}

bool ParamTestNode::operator==(const ParamTestNode & rhs) const {
	return fieldOfArg1 == rhs.fieldOfArg1
		&& conditionNumberOfArg2 == rhs.conditionNumberOfArg2
		&& fieldOfArg2 == rhs.fieldOfArg2;
}

bool ParamTestNode::performTest(const Token & token, const WME & wme) const {
	return token.at(conditionNumberOfArg2).get(fieldOfArg2) == wme.get(fieldOfArg1);
}

ParamTestNodeVector ParamTestNode::generate(const Condition& c, const ConditionVector& condsHigherUp) {
	ParamTestNodeVector result;
	std::unordered_map<std::string, std::pair<size_t, Field::Name>> dict;
	for (size_t i = 0; i < condsHigherUp.size(); ++i) {
		auto&& cond = condsHigherUp.at(i);
		if (cond.getType() == Condition::positive) {
			for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
				auto&& fieldString = cond.get(fieldName);
				if (Field::isParam(fieldString)) {
					auto&& paramName = Field::getParamName(fieldString);
					dict.insert_or_assign(paramName, std::make_pair(i, fieldName));
				}
			}
		}
	}
	std::unordered_set<std::string> checkSet;
	for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
		auto&& fieldString = c.get(fieldName);
		if (Field::isParam(fieldString)) {
			auto&& paramName = Field::getParamName(fieldString);
			auto&& it = dict.find(paramName);
			if (it != dict.end()) {
				result.emplace_back(fieldName, it->second.first, it->second.second);
			}
			else {
				myAssert(checkSet.count(paramName) == 0);
				checkSet.insert(paramName);
			}
		}
	}
	return result;
}

//bool TestNodeVector::performJoinTest(const Token & token, const WME & wme) const {
//	for (auto&& test : *this) {
//		if (!test.performTest(token, wme))
//			return false;
//	}
//	return true;
//}
