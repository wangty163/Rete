#include "Token.h"

#include "VectorPrinter.h"

void Token::print(int level) const {
	VectorPrinter::print(*this, level);
}
