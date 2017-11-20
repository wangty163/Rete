#pragma once

#include <memory>

#include "../Structure/Node.h"
#include "../Structure/WME.h"
#include "../BetaPart/BetaNode.h"
#include "../../HashSupport.h"

class BetaNode;
class AlphaMemory : public Node<WMESet> {
public:
	void addWME(const WME& wme);
	void clearStatus() override;
	void addToChildren(const std::shared_ptr<BetaNode>& ptr);
protected:
	using BetaNodePtr = shared_ptr<BetaNode>;
	std::vector<BetaNodePtr> children;
};

using AlphaMemoryPtr = std::shared_ptr<AlphaMemory>;

DEFINE_STD_HASH_SPECIALIZATION(AlphaMemory);