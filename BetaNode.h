#pragma once

#include "ReteNode.h"
#include "AlphaMemory.h"
#include "ParamTestNode.h"

class BetaNode : public ReteNode {
protected:
	ReteNodePtr leftParent;
	AlphaMemoryPtr rightParent;
	ParamTestNodeVector tests;
public:
	BetaNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent
		, const ParamTestNodeVector& tests);
	void clearStatus() override;
};