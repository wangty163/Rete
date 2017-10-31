#pragma once

#include <memory>

#include "../Structure/Node.h"
#include "../Structure/WME.h"

class AlphaMemory : public Node<WMEVector> {
public:
	void addWME(const WME& wme);
	const WMEVector& getOutput() override;
	void clearStatus() override;
};

using AlphaMemoryPtr = std::shared_ptr<AlphaMemory>;