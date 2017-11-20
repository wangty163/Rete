#include "Net.h"

#include "../BetaPart/JoinNode.h"
#include "../BetaPart/DummyTopNode.h"
#include "../BetaPart/TokenFilterNode.h"

ParamTestNodeVector Net::getTestsFromCondition(Condition c, const ConditionVector & condsHigherUp) {
	ParamTestNodeVector ret;
	auto&& paramTests = ParamTestNode::generate(c, condsHigherUp);
	std::copy(paramTests.begin(), paramTests.end(), std::back_inserter(ret));
	return ret;
}

BetaNodePtr Net::buildOrShareJoinNode(BetaNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = BetaNodePtr(new JoinNode(parent, am, tests, c));
		parent->addToChildren(ret);
		am->addToChildren(ret);
		dict.insert(std::make_pair(node, ret));
		return ret;
	}
	else
		return it->second;
}

BetaNodePtr Net::buildOrShareTokenFilterNode(BetaNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = BetaNodePtr(new TokenFilterNode(parent, am, tests, c, testAtTokenFilterNode));
		parent->addToChildren(ret);
		am->addToChildren(ret);
		dict.insert(std::make_pair(node, ret));
		return ret;
	}
	else
		return it->second;
}

AlphaMemoryPtr Net::buildOrShareAlphaMemory(const Condition & c) {
	auto&& index = c.getIndex();
	if (conditionToAlphaMemory.count(index) == 0) {
		return conditionToAlphaMemory[index] = AlphaMemoryPtr(new AlphaMemory());
	}
	return conditionToAlphaMemory[index];
}

BetaNodePtr Net::buildOrShareNetworkForConditions(BetaNodePtr parent
	, const ConditionVector & conds, ConditionVector condsHigherUp) {
	for (auto&& c : conds) {
		auto&& tests = getTestsFromCondition(c, condsHigherUp);
		auto&& am = buildOrShareAlphaMemory(c);
		if (testAtTokenFilterNode.isNeedFilterToken(c)) {
			parent = buildOrShareTokenFilterNode(parent, am, tests, c);
		}
		else {
			parent = buildOrShareJoinNode(parent, am, tests, c);
		}
		condsHigherUp.push_back(c);
	}
	return parent;
}

Net::Net() : dummyTopNode(BetaNodePtr((new DummyTopNode()))) {
}

size_t i = 0;
void Net::addProduction(const ConditionVector & conditions, const std::vector<Condition>& getter) {
	auto&& curentNode = buildOrShareNetworkForConditions(dummyTopNode, conditions, {});
	auto&& productionNodePtr = ProductionNodePtr(new ProductionNode(agent, curentNode, conditions, getter));
	curentNode->addToChildren(productionNodePtr);
	resultNodes.insert(productionNodePtr);
}

std::vector<ConditionVector> Net::invoke() {
	return agent.getOutput();
}

void Net::clearStatus() {
	for (auto&& pair : conditionToAlphaMemory) {
		pair.second->clearStatus();
	}
	dummyTopNode->clearStatus();
	agent.clearStatus();
	//for (auto&& node : resultNodes) {
	//	node->clearStatus();
	//}
}

void Net::addWME(const WME & wme) {
	std::vector<std::vector<std::string>> vt = {
		{ wme.get(Field::id), Condition::arbitraryString },
		{ wme.get(Field::attr), Condition::arbitraryString },
		{ wme.get(Field::value), Condition::arbitraryString },
	};
	for (auto&& id : vt.at(0)) {
		for (auto&& attr : vt.at(1)) {
			for (auto&& value : vt.at(2)) {
				auto&& it = conditionToAlphaMemory.find(Condition(id, attr, value));
				if (it != conditionToAlphaMemory.end())
					it->second->addWME(wme);
			}
		}
	}
}

void Net::addFunction(const std::string & key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction) {
	testAtTokenFilterNode.insertJudgeFunction(key, judgeFunction);
}

bool StructForHash::operator==(const StructForHash & rhs) const {
	return *parent == *rhs.parent && 
		tests == rhs.tests && 
		*alphaMemory == *rhs.alphaMemory && 
		c == rhs.c;
}

size_t StructForHash::hashCode() const {
	size_t ret = 2166136261;
	ret = (ret * 16777619) ^ hash<BetaNode>()(*parent);
	ret = (ret * 16777619) ^ hash<AlphaMemory>()(*alphaMemory);
	ret = (ret * 16777619) ^ hash<ParamTestNodeVector>()(tests);
	ret = (ret * 16777619) ^ hash<Condition>()(c);
	return ret;
}
