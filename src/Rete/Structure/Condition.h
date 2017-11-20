#pragma once

#include <iostream>

#include "Triples.h"

class Condition : public Triples {
public:
	enum Type {
		positive,
		negetive,
	};
	const static std::string arbitraryString;
	template<typename ID, typename Attr, typename Value>
	Condition(const ID& id, const Attr& attr, const Value& value,
		Type type = positive);
	bool isConstTest(Field::Name fieldName) const;
	Condition getIndex() const;
	const Condition::Type& getType() const;
	void print(int level) const;

	bool operator==(const Condition& rhs) const;
	size_t hashCode() const;
protected:
	Type type;
};

using ConditionVector = std::vector<Condition>;

template<typename ID, typename Attr, typename Value>
inline Condition::Condition(const ID & id, const Attr & attr, const Value & value, Type type)
	: Triples(id, attr, value), type(type) {
}

DEFINE_STD_HASH_SPECIALIZATION(Condition);
