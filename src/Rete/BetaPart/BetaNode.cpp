#include "BetaNode.h"

BetaNode::BetaNode(BetaNodePtr leftParent, AlphaMemoryPtr rightParent, const ParamTestNodeVector & tests)
	: leftParent(leftParent), rightParent(rightParent), tests(tests) {
	// ERROR
	//if (leftParent != nullptr)
	//	leftParent->children.push_back(BetaNodePtr(this));
	//if (rightParent != nullptr)
	//	rightParent->addToChildren(BetaNodePtr(this));
}

void BetaNode::addToOutput(Token token, const WME& wme) {
	token.push_back(wme);
	auto&& outputPair = output.insert(token);
	auto&& recordPair = record[token].insert(wme);
	myAssert(outputPair.second == recordPair.second);
	if (outputPair.second)
		for (auto&& child : children)
			child->leftActive(token);
}

void BetaNode::removeFromOutput(Token token, const WME & wme) {
	token.push_back(wme);
	auto&& outputReturn = output.erase(token);
	auto&& recordReturn = record[token].erase(wme);
	myAssert(outputReturn == recordReturn);
	if (outputReturn > 0)
		for (auto&& child : children)
			child->leftDeactive(token);
}

void BetaNode::addToChildren(const BetaNodePtr & ptr) {
	children.push_back(ptr);
}

void BetaNode::leftDeactive(const Token & token) {
	for (auto&& wme : record[token]) {
		removeFromOutput(token, wme);
	}
	record.erase(token);
}

void BetaNode::clearStatus() {
	if (!output.empty()) {
		output.clear();
		record.clear();
		for (auto&& child : children)
			child->clearStatus();
	}
}
