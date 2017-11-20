#include "WME.h"

void WMESet::add(const WME & wme) {
	insert(wme);
}

void WMESet::remove(const WME & wme) {
	erase(wme);
}
