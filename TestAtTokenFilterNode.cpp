#include "TestAtTokenFilterNode.h"

#include "CStringOp.h"
#include "PublicDefine.h"

using JudgeFunctionType = TestAtTokenFilterNode::JudgeFunctionType;
const static std::unordered_map<std::string, JudgeFunctionType> TestAtTokenFilterNode::dict = {
	{
		"numeric-greater-than",
		[](const std::string& left, const std::string& right) {
			double leftValue = CStringOp::FromString<double>(left);
			double rightValue = CStringOp::FromString<double>(right);
			return leftValue > rightValue;
		}
	},
};

bool TestAtTokenFilterNode::performTest(const Condition & c)
{
	using namespace Field;
	auto&& it = dict.find(c.get(attr));
	myAssert(it != dict.end());
	return it->second(c.get(id), c.get(value));
}

bool TestAtTokenFilterNode::isNeedFilterToken(const Condition & c)
{
	return dict.count(c.get(Field::attr)) > 0;
}
