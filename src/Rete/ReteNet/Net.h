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
			size_t ret = 2166136261;
			ret = (ret * 16777619) ^ hash<ReteNode>()(*obj.parent);
			ret = (ret * 16777619) ^ hash<AlphaMemory>()(*obj.alphaMemory);
			ret = (ret * 16777619) ^ hash<ParamTestNodeVector>()(obj.tests);
			ret = (ret * 16777619) ^ hash<Condition>()(obj.c);
			return ret;
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
	void addProduction(const ConditionVector& conditions, const std::vector<Condition>& getter);
	std::vector<ConditionVector> invoke();
	void clearStatus();
	void addWME(const WME& wme);
	void addFunction(const std::string& key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction);
};