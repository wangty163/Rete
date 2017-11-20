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
	bool performTest(const Token& token, const WME& wme) const;
	static ParamTestNodeVector generate(const Condition& c, const ConditionVector& condsHigherUp);

	bool operator== (const ParamTestNode& rhs) const;
	size_t hashCode() const;
};

DEFINE_STD_HASH_SPECIALIZATION(ParamTestNode);

using ParamTestNodeVector = std::vector<ParamTestNode>;