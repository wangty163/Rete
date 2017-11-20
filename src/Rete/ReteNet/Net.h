#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iterator>

#include "../../PublicDefine.h"
#include "../TestNode/TestAtTokenFilterNode.h"
#include "../BetaPart/BetaNode.h"
#include "../Structure/Condition.h"
#include "../AlphaPart/AlphaMemory.h"
#include "../BetaPart/ProductionNode.h"
#include "../TestNode/ParamTestNode.h"

struct StructForHash {
	BetaNodePtr parent;
	AlphaMemoryPtr alphaMemory;
	ParamTestNodeVector tests;
	Condition c;

	bool operator== (const StructForHash& rhs) const;
	size_t hashCode() const;
};

DEFINE_STD_HASH_SPECIALIZATION(StructForHash);

class Net {
	BetaNodePtr dummyTopNode;
	Agent agent;
	TestAtTokenFilterNode testAtTokenFilterNode;
	std::unordered_map<Condition, AlphaMemoryPtr> conditionToAlphaMemory;
	std::unordered_map<StructForHash, BetaNodePtr> dict;

	ParamTestNodeVector getTestsFromCondition(Condition c
		, const ConditionVector& condsHigherUp);
	BetaNodePtr buildOrShareJoinNode(BetaNodePtr parent, AlphaMemoryPtr am
		, const ParamTestNodeVector& tests, const Condition& c);
	BetaNodePtr buildOrShareTokenFilterNode(BetaNodePtr parent, AlphaMemoryPtr am
		, const ParamTestNodeVector & tests, const Condition& c);
	AlphaMemoryPtr buildOrShareAlphaMemory(const Condition& c);
	BetaNodePtr buildOrShareNetworkForConditions(BetaNodePtr parent
		, const ConditionVector& conds, ConditionVector condsHigherUp);

	std::unordered_set<ProductionNodePtr> resultNodes;
public:
	Net();
	void addProduction(const ConditionVector& conditions, const std::vector<Condition>& getter);
	std::vector<ConditionVector> invoke();
	void clearStatus();
	void addWME(const WME& wme);
	void addFunction(const std::string& key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction);
};