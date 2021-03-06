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

ReteNodePtr Net::buildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = ReteNodePtr((ReteNode*)new JoinNode(parent, am, tests, c));
		dict.insert(std::make_pair(node, ret));
		return ret;
	}
	else
		return it->second;
}

ReteNodePtr Net::buildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = ReteNodePtr(new TokenFilterNode(parent, am, tests, c, testAtTokenFilterNode));
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

ReteNodePtr Net::buildOrShareNetworkForConditions(ReteNodePtr parent
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

Net::Net() : dummyTopNode(ReteNodePtr((ReteNode*)(new DummyTopNode()))) {
}

size_t i = 0;
void Net::addProduction(const ConditionVector & conditions, const std::vector<Condition>& getter) {
	auto&& curentNode = buildOrShareNetworkForConditions(dummyTopNode, conditions, {});
	resultNodes.insert(ProductionNodePtr(new ProductionNode(curentNode, conditions, getter)));
}

std::vector<ConditionVector> Net::invoke() {
	std::vector<ConditionVector> ret;
	for (auto&& node : resultNodes) {
		auto&& infos = node->getOutputInfos();
		std::copy(infos.begin(), infos.end(), std::back_inserter(ret));
	}
	return ret;
}

void Net::clearStatus() {
	for (auto&& node : resultNodes) {
		node->clearStatus();
	}
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