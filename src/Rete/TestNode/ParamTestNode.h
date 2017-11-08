#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../Structure/Condition.h"
#include "../Structure/Token.h"
#include "../Structure/WME.h"

struct ParamTestNode {
	using ParamTestNodeVector = std::vector<ParamTestNode>;

	Field::Name fieldOfArg1;
	size_t conditionNumberOfArg2;
	Field::Name fieldOfArg2;

	ParamTestNode(Field::Name fieldOfArg1, size_t conditionNumberOfArg2
		, Field::Name fieldOfArg2);
	bool operator== (const ParamTestNode& rhs) const;
	bool performTest(const Token& token, const WME& wme) const;
	static ParamTestNodeVector generate(const Condition& c, const ConditionVector& condsHigherUp);
};

using ParamTestNodeVector = std::vector<ParamTestNode>;

namespace std {
	template<>
	struct hash<ParamTestNode> {
		size_t operator() (const ParamTestNode& node) const {
			size_t ret = 2166136261;
			ret = (ret * 16777619) ^ hash<int>()(node.fieldOfArg1);
			ret = (ret * 16777619) ^ hash<size_t>()(node.conditionNumberOfArg2);
			ret = (ret * 16777619) ^ hash<int>()(node.fieldOfArg2);
			return ret;
		}
	};
	template<>
	struct hash<ParamTestNodeVector> {
		size_t operator() (const ParamTestNodeVector& nodes) const {
			size_t ret = 2166136261;
			for (auto&& node : nodes)
				ret = (ret * 16777619) ^ hash<ParamTestNode>()(node);
			return ret;
		}
	};
}

//class TestNodeVector : public PrintableVector<ParamTestNode> {
//public:
//	bool performJoinTest(const Token& token, const WME& wme) const;
//};