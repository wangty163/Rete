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
protected:
	Type type;
};

using ConditionVector = std::vector<Condition>;

namespace std {
	template <>
	struct hash<Condition> {
		size_t operator()(const Condition& c) const {
			size_t ret = 2166136261;
			ret = (ret * 16777619) ^ hash<string>()(c.get(Field::id));
			ret = (ret * 16777619) ^ hash<string>()(c.get(Field::attr));
			ret = (ret * 16777619) ^ hash<string>()(c.get(Field::value));
			ret = (ret * 16777619) ^ hash<Condition::Type>()(c.getType());
			return ret;
		}
	};
}

template<typename ID, typename Attr, typename Value>
inline Condition::Condition(const ID & id, const Attr & attr, const Value & value, Type type)
	: Triples(id, attr, value), type(type) {
}
