#include "DummyTopNode.h"

#include "../../PublicDefine.h"

DummyTopNode::DummyTopNode()
	: BetaNode(nullptr, nullptr, {}) {
	output.insert(Token());
}

void DummyTopNode::clearStatus() {
	for (auto&& child : children)
		child->clearStatus();
}

void DummyTopNode::leftActive(const Token & token) {
	myAssert(false);
}

void DummyTopNode::rightActive(const WME & wme) {
	myAssert(false);
}
