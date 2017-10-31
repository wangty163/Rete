#include "ProductionNode.h"

#include "../TestNode/ParamTestNode.h"
#include "../Utils/VectorPrinter.h"

ProductionNode::ProductionNode(ReteNodePtr leftParent, const ConditionVector & conds, const std::string& comment)
	: ReteNode(false), leftParent(leftParent), conds(conds), comment(comment) {
}

void ProductionNode::print(int level) const {
	if (!output.empty()) {
		std::cout << "matched productions:" << std::endl;
		std::cout << comment << std::endl;
		//VectorPrinter::print(conds, level);
		std::cout << "matched woking memory elements:" << std::endl;
		VectorPrinter::print(output, level);
	}
}

const ConditionVector & ProductionNode::getConds() {
	return conds;
}

TokenVector & ProductionNode::getOutput() {
	if (!isReadyForOutput) {
		output = leftParent->getOutput();
		isReadyForOutput = true;

		//if (!output.empty()) {
		//	ConditionVector condsHigherUp;
		//	for (size_t i = 0; i < conds.size(); ++i) {
		//		auto&& c = conds.at(i);
		//		if (c.getType() == Condition::negetive) {
		//			for (auto&& test : ParamTestNode::generate(c, conds)) {
		//				for (size_t j = 0; j < output.size(); ++j) {
		//					for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
		//						output.at(j).at(i).set(fieldName, c.get(fieldName));
		//					}
		//					output.at(j).at(i).set(test->fieldOfArg1
		//						, output.at(j).at(test->conditionNumberOfArg2).get(test->fieldOfArg2));
		//				}
		//			}
		//			for (size_t j = 0; j < output.size(); ++j) {
		//				output.at(j).at(i).set(Field::attr, c.get(Field::attr) + "_neq");
		//			}
		//		}
		//	}
		//}
	}
	return output;
}

void ProductionNode::clearStatus() {
	if (isReadyForOutput) {
		isReadyForOutput = false;
		output.clear();
		leftParent->clearStatus();
	}
}
