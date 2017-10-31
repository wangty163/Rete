#pragma once

#include <iterator>

#include "ReteNode.h"
#include "../Structure/Condition.h"

class ProductionNode : public ReteNode {
	ReteNodePtr leftParent;
	ConditionVector conds;
	std::string comment;
public:
	ProductionNode(ReteNodePtr leftParent, const ConditionVector& conds, const std::string& comment);
	void print(int level = 0) const;
	const ConditionVector& getConds();
	virtual TokenVector& getOutput() override;
	virtual void clearStatus() override;
};

using ProductionNodePtr = std::shared_ptr<ProductionNode>;