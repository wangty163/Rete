#include "JoinNode.h"

#include "../../PublicDefine.h"

JoinNode::JoinNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent
	, const ParamTestNodeVector& tests, const Condition& c)
	: BetaNode(leftParent, rightParent, tests) {
	switch (c.getType())
	{
	case Condition::positive:
		isPositive = true;
		break;
	case Condition::negetive:
		isPositive = false;
		break;
	default:
		myAssert(false);
	}
}

const TokenVector & JoinNode::getOutput() {
	if (!isReadyForOutput) {
		// Can Be Optimized
		for (auto&& token : leftParent->getOutput()) {
			if (isPositive) {
				for (auto&& wme : rightParent->getOutput()) {
					bool passAllCheck = true;
					for (auto&& test : tests) {
						if (!test.performTest(token, wme)) {
							passAllCheck = false;
							break;
						}
					}
					if (passAllCheck) {
						output.push_back(token);
						output.back().push_back(wme);
					}
				}
			}
			else {
				bool passNoneCheck = true;
				for (auto&& wme : rightParent->getOutput()) {
					for (auto&& test : tests) {
						if (test.performTest(token, wme)) {
							passNoneCheck = false;
							break;
						}
					}
					if (!passNoneCheck)
						break;
				}
				if (passNoneCheck) {
					output.push_back(token);
					output.back().push_back({ "?", "?", "?" });
				}
			}
		}
		isReadyForOutput = true;
	}
	return output;
}