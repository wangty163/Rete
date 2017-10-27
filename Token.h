#pragma once

#include "WME.h"
#include "PrintableVector.h"

class Token : public PrintableVector<WME> {
public:
	Token(const std::initializer_list<WME>& lst);
};

class TokenVector : public PrintableVector<Token> {
public:
	TokenVector();
	TokenVector(const std::initializer_list<Token>& lst);
};