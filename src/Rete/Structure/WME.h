#pragma once

#include "Triples.h"
#include "../../PublicDefine.h"

class WME : public Triples {
public:
	template<typename ID, typename Attr, typename Value>
	WME(const ID& id, const Attr& attr, const Value& value);
};

class WMEVector : public std::vector<WME> {
};

template<typename ID, typename Attr, typename Value>
inline WME::WME(const ID & id, const Attr & attr, const Value & value)
	: Triples({ id, attr, value }) {
	myAssert(!Field::isParam(fields.at(0)));
	myAssert(!Field::isParam(fields.at(1)));
	myAssert(!Field::isParam(fields.at(2)));
}
