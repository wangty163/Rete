#pragma once

#include <memory>

#include "../Structure/Node.h"
#include "../Structure/Token.h"

class ReteNode : public Node<TokenVector> {
protected:
	bool isReadyForOutput;
public:
	ReteNode(bool isReadyForOutput);
};

using ReteNodePtr = std::shared_ptr<ReteNode>;