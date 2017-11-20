#pragma once

#include "../Structure/WME.h"
#include "../Structure/Token.h"
#include "../Structure/Condition.h"
#include "../BetaPart/ProductionNode.h"

class ProductionNode;
class Agent {
public:
	using ProductionNodePtr = std::shared_ptr<ProductionNode>;
	void addToOutput(const Token& token, const ConditionVector& conditionVector);
	void removeFromOutput(const Token& token);
	void clearStatus();
	std::vector<ConditionVector> getOutput() const;
private:
	std::unordered_map<Token, std::vector<ConditionVector>> dict;
};