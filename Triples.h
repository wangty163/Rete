#pragma once

#include <vector>
#include <string>

#include "Field.h"

class Triples
{
protected:
	std::vector<std::string> fields;
	size_t getIndex(Field::Name fieldName) const;
public:
	template<typename ID, typename Attr, typename Value>
	Triples(const ID& id, const Attr& attr, const Value& value);
	void set(Field::Name fieldName, const std::string& v);
	const std::string& get(Field::Name fieldName) const;
	bool operator== (const Triples &rhs) const;
	void print(size_t level) const;
};

template<typename ID, typename Attr, typename Value>
inline Triples::Triples(const ID & id, const Attr & attr, const Value & value)
	: fields({ CStringOp::ToString(id), CStringOp::ToString(attr), CStringOp::ToString(value) }) {
}