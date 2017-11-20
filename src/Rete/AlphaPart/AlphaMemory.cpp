#include "AlphaMemory.h"

void AlphaMemory::addWME(const WME & wme) {
	output.add(wme);
	for (auto&& child : children) {
		child->rightActive(wme);
	}
}

void AlphaMemory::clearStatus() {
	if (!output.empty()) {
		output.clear();
		for (auto&& child : children) {
			child->clearStatus();
		}
	}
}

void AlphaMemory::addToChildren(const std::shared_ptr<BetaNode> & ptr) {
	children.push_back(ptr);
}
