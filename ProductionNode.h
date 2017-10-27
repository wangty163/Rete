#pragma once

#include <iterator>

#include "ReteNode.h"
#include "Condition.h"

class ProductionNode : public ReteNode {
	ReteNodePtr leftParent;
	ConditionVector conds;
public:
	ProductionNode(ReteNodePtr leftParent, const ConditionVector& conds);
	void print(size_t level = 0) const;
	const ConditionVector& getConds();
	virtual TokenVector& getOutput() override;
	virtual void clearStatus() override;
};

using ProductionNodePtr = std::shared_ptr<ProductionNode>;