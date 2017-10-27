#pragma once

#include <memory>

#include "Node.h"
#include "Token.h"

class ReteNode : public Node<TokenVector> {
protected:
	bool isReadyForOutput;
public:
	ReteNode(bool isReadyForOutput);
	virtual void clearStatus() = 0;
};

using ReteNodePtr = std::shared_ptr<ReteNode>;