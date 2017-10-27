#include "Condition.h"

#include "CStringOp.h"

const std::string Condition::arbitraryString = "*"; 


bool Condition::isConstTest(Field::Name fieldName) const {
	return !Field::isParam(this->get(fieldName));
}

Condition Condition::getIndex() const {
	Condition ret = *this;
	if (!ret.isConstTest(Field::id))
		ret.fields.at(0) = Condition::arbitraryString;
	if (!ret.isConstTest(Field::attr))
		ret.fields.at(1) = Condition::arbitraryString;
	if (!ret.isConstTest(Field::value))
		ret.fields.at(2) = Condition::arbitraryString;
	return ret;
}

const Condition::Type & Condition::getType() const {
	return type;
}

void Condition::print(size_t level) const {
	std::string prefix = CStringOp::repeat("\t", level);
	std::cout << prefix << fields.at(0) << ","
		<< (type == negetive ? "~" : "")
		<< fields.at(1)
		<< "," << fields.at(2) << std::endl;
}

ConditionVector::ConditionVector(const std::initializer_list<Condition>& lst)
	: PrintableVector<Condition>(lst) {
}

