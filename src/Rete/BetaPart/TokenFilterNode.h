#pragma once

#include "BetaNode.h"
#include "../TestNode/TestAtTokenFilterNode.h"

class TokenFilterNode : public BetaNode {
public:
	TokenFilterNode(BetaNodePtr leftParent, AlphaMemoryPtr rightParent,
		const ParamTestNodeVector & tests, const Condition& c, TestAtTokenFilterNode& testAtTokenFilterNode);
	void leftActive(const Token& token) override;
	void rightActive(const WME& wme) override;
private:
	Condition c;
	bool isPositive;
	TestAtTokenFilterNode& testAtTokenFilterNode;
};