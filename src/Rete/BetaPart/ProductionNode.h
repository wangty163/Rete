#pragma once

#include <iterator>

#include "BetaNode.h"
#include "../Structure/Condition.h"
#include "../ReteNet/Agent.h"

class Agent;
class ProductionNode : public BetaNode {
	using PositionIndexer = std::pair<size_t, Field::Name>;

	Agent& agent;
	ConditionVector conds, infoGetter;
	std::vector<std::pair<PositionIndexer, PositionIndexer>> indexerForInfoGetter;

	ConditionVector getOutputInfo(const Token& token) const;
	void buildIndexer();
public:
	ProductionNode(Agent& agent, BetaNodePtr leftParent, const ConditionVector& conds, const ConditionVector& infoGetter);
	void print(int level = 0) const;

	void leftActive(const Token& token) override;
	void leftDeactive(const Token& token) override;
	void rightActive(const WME& wme) override;
};

DEFINE_STD_HASH_SPECIALIZATION(ProductionNode);

//using ProductionNodePtr = std::shared_ptr<ProductionNode>;

class ProductionNodePtr : public std::shared_ptr<ProductionNode> {
public:
	ProductionNodePtr(ProductionNode* ptr);

	bool operator==(const ProductionNodePtr& rhs) const;
	size_t hashCode() const;
};

DEFINE_STD_HASH_SPECIALIZATION(ProductionNodePtr);