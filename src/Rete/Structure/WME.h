#pragma once

#include <unordered_set>

#include "Triples.h"
#include "../../PublicDefine.h"
#include "../../HashSupport.h"

class WME : public Triples {
public:
	template<typename ID, typename Attr, typename Value>
	WME(const ID& id, const Attr& attr, const Value& value);
};

template<typename ID, typename Attr, typename Value>
inline WME::WME(const ID & id, const Attr & attr, const Value & value)
	: Triples({ id, attr, value }) {
	myAssert(!Field::isParam(fields.at(0)));
	myAssert(!Field::isParam(fields.at(1)));
	myAssert(!Field::isParam(fields.at(2)));
}
DEFINE_STD_HASH_SPECIALIZATION(WME);

class WMESet : public std::unordered_set<WME> {
public:
	void add(const WME& wme);
	void remove(const WME& wme);
};