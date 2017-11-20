#include "Token.h"

#include "../Utils/VectorPrinter.h"

void Token::print(int level) const {
	VectorPrinter::print(*this, level);
}

size_t Token::hashCode() const {
	size_t ret = 0;
	for (auto&& wme : *this)
		ret = (ret * 16777619) ^ wme.hashCode();
	return ret;
}
