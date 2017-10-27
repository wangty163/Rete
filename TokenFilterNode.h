#pragma once

#include "BetaNode.h"

class TokenFilterNode : public BetaNode {
public:
	TokenFilterNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent,
		const ParamTestNodeVector & tests, const Condition& c);
	const TokenVector& getOutput() override;
private:
	Condition c;
	bool isPositive;
};