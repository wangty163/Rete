#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iterator>

#include "PublicDefine.h"
#include "TestAtTokenFilterNode.h"
#include "ReteNode.h"
#include "Condition.h"
#include "AlphaMemory.h"
#include "ProductionNode.h"
#include "ParamTestNode.h"

struct StructForHash {
	ReteNodePtr parent;
	AlphaMemoryPtr alphaMemory;
	ParamTestNodeVector tests;
	Condition c;
	bool operator== (const StructForHash& rhs) const;
};

namespace std {
	template<>
	struct hash<StructForHash> {
		std::size_t operator()(const StructForHash& obj) const {
			return hash<ReteNodePtr>()(obj.parent)
				^ hash<AlphaMemoryPtr>()(obj.alphaMemory)
				^ hash<ParamTestNodeVector>()(obj.tests)
				^ hash<Condition>()(obj.c);
		}
	};
}

class Net {
	ReteNodePtr dummyTopNode;
	TestAtTokenFilterNode testAtTokenFilterNode;
	std::unordered_map<Condition, AlphaMemoryPtr> conditionToAlphaMemory;
	std::unordered_map<StructForHash, ReteNodePtr> dict;

	ParamTestNodeVector getTestsFromCondition(Condition c
		, const ConditionVector& condsHigherUp);
	ReteNodePtr buildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am
		, const ParamTestNodeVector& tests, const Condition& c);
	ReteNodePtr buildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am
		, const ParamTestNodeVector & tests, const Condition& c);
	AlphaMemoryPtr buildOrShareAlphaMemory(const Condition& c);
	ReteNodePtr buildOrShareNetworkForConditions(ReteNodePtr parent
		, const ConditionVector& conds, ConditionVector condsHigherUp);

	std::unordered_set<ProductionNodePtr> resultNodes;
public:
	Net();
	void addProduction(const ConditionVector& conditions, const std::string& comment);
	void invoke();
	void addWME(const WME& wme);
	void addFunction(const std::string& key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction);
};