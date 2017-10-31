#pragma once

#include "BetaNode.h"
#include "TestAtTokenFilterNode.h"

class TokenFilterNode : public BetaNode {
public:
	TokenFilterNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent,
		const ParamTestNodeVector & tests, const Condition& c, TestAtTokenFilterNode& testAtTokenFilterNode);
	const TokenVector& getOutput() override;
private:
	Condition c;
	bool isPositive;
	TestAtTokenFilterNode& testAtTokenFilterNode;
};