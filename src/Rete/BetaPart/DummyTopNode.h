#pragma once

#include "ReteNode.h"

struct DummyTopNode : public ReteNode {
public:
	DummyTopNode();
	void clearStatus() override;
	const TokenVector& getOutput() override;
};