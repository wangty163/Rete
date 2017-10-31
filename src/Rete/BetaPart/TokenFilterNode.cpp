#include "TokenFilterNode.h"

#include "../TestNode/TestAtTokenFilterNode.h"
#include "../../PublicDefine.h"

TokenFilterNode::TokenFilterNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent,
	const ParamTestNodeVector & tests, const Condition& c, TestAtTokenFilterNode& testAtTokenFilterNode)
	: BetaNode(leftParent, rightParent, tests), c(c), testAtTokenFilterNode(testAtTokenFilterNode) {
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

const TokenVector & TokenFilterNode::getOutput() {
	if (!isReadyForOutput) {
		// Can Be Optimized
		for (auto&& token : leftParent->getOutput()) {
			for (auto&& test : tests) {
				c.set(
					test.fieldOfArg1, 
					token.at(test.conditionNumberOfArg2).get(test.fieldOfArg2)
				);
			}
			if ((isPositive ^ testAtTokenFilterNode.performTest(c)) == 0) {
				output.push_back(token);
				output.back().push_back({ 
					c.get(Field::id), 
					(isPositive ? "" : "~") + c.get(Field::attr),
					c.get(Field::value)
				});
			}
		}
		isReadyForOutput = true;
	}
	return output;
}
