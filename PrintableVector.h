#pragma once

#include "CStringOp.h"

template<typename T>
class PrintableVector : public std::vector<T> {
public:
	PrintableVector();
	PrintableVector(const std::initializer_list<T>& lst);
	const void print(size_t level) const;
};

/********************************************************************************/
template<typename T>
inline PrintableVector<T>::PrintableVector()
{
}

template<typename T>
inline PrintableVector<T>::PrintableVector(const std::initializer_list<T>& lst)
	: std::vector<T>(lst)
{
}

template<typename T>
inline const void PrintableVector<T>::print(size_t level) const
{
	std::string prefix = CStringOp::repeat("\t", level);
	std::cout << prefix << "{" << std::endl;
	for (auto&& v : *this) {
		v.print(level + 1);
	}
	std::cout << prefix << "}" << std::endl;
}