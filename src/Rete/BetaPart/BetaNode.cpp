#include "BetaNode.h"

BetaNode::BetaNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent
	, const ParamTestNodeVector & tests)
	: ReteNode(false), leftParent(leftParent), rightParent(rightParent), tests(tests) {
}

void BetaNode::clearStatus() {
	if (isReadyForOutput) {
		isReadyForOutput = false;
		output.clear();
		leftParent->clearStatus();
		rightParent->clearStatus();
	}
}
