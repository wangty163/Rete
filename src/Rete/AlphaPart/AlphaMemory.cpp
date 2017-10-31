#include "AlphaMemory.h"

void AlphaMemory::addWME(const WME & wme) {
	output.push_back(wme);
}

const WMEVector & AlphaMemory::getOutput() {
	return output;
}

void AlphaMemory::clearStatus() {
	output.clear();
}
