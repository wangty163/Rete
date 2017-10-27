#include "Token.h"

#include "VectorPrinter.h"

void Token::print(size_t level) const {
	VectorPrinter::print(*this, level);
}
