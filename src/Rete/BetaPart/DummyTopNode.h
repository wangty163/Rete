#pragma once

#include "BetaNode.h"

struct DummyTopNode : public BetaNode {
public:
	DummyTopNode();
	void clearStatus() override;
	void leftActive(const Token& token) override;
	void rightActive(const WME& wme) override;
};