#pragma once

#include <iostream>

#include "Triples.h"
//#include "PrintableVector.h"

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
protected:
	Type type;
};

using ConditionVector = std::vector<Condition>;

namespace std {
	template <>
	struct hash<Condition> {
		size_t operator()(const Condition& c) const {
			return (hash<string>()(c.get(Field::id))
				^ (hash<string>()(c.get(Field::attr)) >> 1)
				^ (hash<string>()(c.get(Field::value)) << 1));
		}
	};
}

template<typename ID, typename Attr, typename Value>
inline Condition::Condition(const ID & id, const Attr & attr, const Value & value, Type type)
	: Triples(id, attr, value), type(type) {
}
