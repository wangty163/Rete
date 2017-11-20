#include "JoinNode.h"

#include "../../PublicDefine.h"

JoinNode::JoinNode(BetaNodePtr leftParent, AlphaMemoryPtr rightParent
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

void JoinNode::leftActive(const Token & token) {
	// Can Be Optimized
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
				addToOutput(token, wme);
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
			addToOutput(token, { "?", "?", "?" });
		}
	}
}

void JoinNode::rightActive(const WME & wme) {
	if (isPositive) {
		// Can Be Optimized
		for (auto&& token : leftParent->getOutput()) {
			bool passAllCheck = true;
			for (auto&& test : tests) {
				if (!test.performTest(token, wme)) {
					passAllCheck = false;
					break;
				}
			}
			if (passAllCheck) {
				addToOutput(token, wme);
			}
		}
	}
	else {
		std::vector<Token> toBeDeleted;
		for (auto&& token : output) {
			for (auto&& test : tests) {
				if (test.performTest(token, wme)) {
					toBeDeleted.push_back(token);
					break;
				}
			}
		}
		for (auto&& token : toBeDeleted) {
			WME wme = token.back();
			token.pop_back();
			removeFromOutput(token, wme);
		}
	}
}