#pragma once

#include <iterator>

#include "BetaNode.h"
#include "../Structure/Condition.h"

class JoinNode : public BetaNode {
public:
	JoinNode(BetaNodePtr leftParent, AlphaMemoryPtr rightParent
		, const ParamTestNodeVector& tests, const Condition& c);
	void leftActive(const Token& token) override;
	void rightActive(const WME& wme) override;
private:
	bool isPositive;
};