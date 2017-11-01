#pragma once

#include <iterator>

#include "ReteNode.h"
#include "../Structure/Condition.h"

class ProductionNode : public ReteNode {
	using PositionIndexer = std::pair<size_t, Field::Name>;

	ReteNodePtr leftParent;
	ConditionVector conds, infoGetter;
	std::vector<ConditionVector> outputInfos;
	std::vector<std::pair<PositionIndexer, PositionIndexer>> indexerForInfoGetter;

	ConditionVector getOutputInfo(const Token& token) const;
	void buildIndexer();
public:
	ProductionNode(ReteNodePtr leftParent, const ConditionVector& conds, const ConditionVector& infoGetter);
	void print(int level = 0) const;
	const ConditionVector& getConds();
	virtual TokenVector& getOutput() override;
	virtual void clearStatus() override;
	std::vector<ConditionVector> getOutputInfos();
};

using ProductionNodePtr = std::shared_ptr<ProductionNode>;