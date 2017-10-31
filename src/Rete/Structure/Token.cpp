#include "Token.h"

#include "../Utils/VectorPrinter.h"

void Token::print(int level) const {
	VectorPrinter::print(*this, level);
}
