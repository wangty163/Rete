#pragma once

#include "../AlphaPart/AlphaMemory.h"
#include "../TestNode/ParamTestNode.h"

class AlphaMemory;
class BetaNode : public Node<TokenSet> {
public:
	using BetaNodePtr = std::shared_ptr<BetaNode>;
	using AlphaMemoryPtr = std::shared_ptr<AlphaMemory>;

	BetaNode(BetaNodePtr leftParent, AlphaMemoryPtr rightParent, const ParamTestNodeVector& tests);

	void addToOutput(Token token, const WME& wme);
	void removeFromOutput(Token token, const WME& wme);
	void addToChildren(const BetaNodePtr& ptr);

	virtual void leftDeactive(const Token& token);
	virtual void leftActive(const Token& token) = 0;
	virtual void rightActive(const WME& wme) = 0;
	virtual void clearStatus() override;
protected:
	BetaNodePtr leftParent;
	AlphaMemoryPtr rightParent;
	ParamTestNodeVector tests;
	std::vector<BetaNodePtr> children;
	std::unordered_map<Token, std::unordered_set<WME>> record;
};

using BetaNodePtr = BetaNode::BetaNodePtr;

DEFINE_STD_HASH_SPECIALIZATION(BetaNode);