#include "ProductionNode.h"

#include "../TestNode/ParamTestNode.h"
#include "../Utils/VectorPrinter.h"

ConditionVector ProductionNode::getOutputInfo(const Token & token) const {
	ConditionVector ret = infoGetter;
	for (auto&& pair : indexerForInfoGetter) {
		auto&& value = token.at(pair.first.first).get(pair.first.second);
		ret.at(pair.second.first).set(pair.second.second, value);
	}
	return ret;
}

void ProductionNode::buildIndexer() {
	using namespace Field;
	std::unordered_map<std::string, std::vector<PositionIndexer>> dict;
	for (size_t i = 0; i < infoGetter.size(); ++i) {
		for (auto&& field : { id, attr, value }) {
			auto&& fieldString = infoGetter.at(i).get(field);
			if (Field::isParam(fieldString)) {
				dict[fieldString].emplace_back(i, field);
			}
		}
	}
	for (size_t i = 0; i < conds.size(); ++i) {
		for (auto&& field : { id, attr, value }) {
			auto&& fieldString = conds.at(i).get(field);
			auto&& it = dict.find(fieldString);
			if (it != dict.end()) {
				for (auto&& positionOfInfoGetter : it->second) {
					indexerForInfoGetter.emplace_back(std::make_pair(i, field), positionOfInfoGetter);
				}
				dict.erase(it);
			}
		}
	}
	myAssert(dict.empty());
}

ProductionNode::ProductionNode(Agent& agent, BetaNodePtr leftParent,
	const ConditionVector & conds, const ConditionVector& infoGetter)
	: BetaNode(leftParent, nullptr, {}), agent(agent), conds(conds), infoGetter(infoGetter) {
	buildIndexer();
}

void ProductionNode::print(int level) const {
	if (!output.empty()) {
		std::cout << "matched productions:" << std::endl;
		VectorPrinter::print(conds, level);
		//std::cout << "matched woking memory elements:" << std::endl;
		//VectorPrinter::print(output, level);
	}
}

//TokenVector & ProductionNode::getOutput() {
//	if (!isReadyForOutput) {
//		output = leftParent->getOutput();
//		isReadyForOutput = true;
//
//
//		//if (!output.empty()) {
//		//	ConditionVector condsHigherUp;
//		//	for (size_t i = 0; i < conds.size(); ++i) {
//		//		auto&& c = conds.at(i);
//		//		if (c.getType() == Condition::negetive) {
//		//			for (auto&& test : ParamTestNode::generate(c, conds)) {
//		//				for (size_t j = 0; j < output.size(); ++j) {
//		//					for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
//		//						output.at(j).at(i).set(fieldName, c.get(fieldName));
//		//					}
//		//					output.at(j).at(i).set(test->fieldOfArg1
//		//						, output.at(j).at(test->conditionNumberOfArg2).get(test->fieldOfArg2));
//		//				}
//		//			}
//		//			for (size_t j = 0; j < output.size(); ++j) {
//		//				output.at(j).at(i).set(Field::attr, c.get(Field::attr) + "_neq");
//		//			}
//		//		}
//		//	}
//		//}
//	}
//	return output;
//}

void ProductionNode::leftActive(const Token & token) {
	agent.addToOutput(token, getOutputInfo(token));
}

void ProductionNode::leftDeactive(const Token & token) {
	BetaNode::leftDeactive(token);
	agent.removeFromOutput(token);
}

void ProductionNode::rightActive(const WME & wme) {
	myAssert(false);
}

ProductionNodePtr::ProductionNodePtr(ProductionNode * ptr)
	: std::shared_ptr<ProductionNode>(ptr) {
}

bool ProductionNodePtr::operator==(const ProductionNodePtr & rhs) const {
	return *get() == *rhs;
}

size_t ProductionNodePtr::hashCode() const {
	return get()->hashCode();
}
