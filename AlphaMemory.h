#pragma once

#include <memory>

#include "Node.h"
#include "WME.h"

class AlphaMemory : public Node<WMEVector> {
public:
	void addWME(const WME& wme);
	const WMEVector& getOutput() override;
};

using AlphaMemoryPtr = std::shared_ptr<AlphaMemory>;