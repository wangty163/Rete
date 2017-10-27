#include "Token.h"

Token::Token(const std::initializer_list<WME>& lst)
	: PrintableVector<WME>(lst) {
}

TokenVector::TokenVector() {
}

TokenVector::TokenVector(const std::initializer_list<Token>& lst)
	: PrintableVector<Token>(lst) {
}
