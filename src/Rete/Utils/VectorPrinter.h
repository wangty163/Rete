#pragma once

#include "../../StringOp.h"

namespace VectorPrinter {
	template<typename T>
	void print(const std::vector<T>& vt, int level);
};

template<typename T>
inline void VectorPrinter::print(const std::vector<T>& vt, int level)
{
	std::string prefix = CStringOp::repeat("\t", level);
	std::cout << prefix << "{" << std::endl;
	for (auto&& v : vt) {
		v.print(level + 1);
	}
	std::cout << prefix << "}" << std::endl;
}