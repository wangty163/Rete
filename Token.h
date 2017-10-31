#pragma once

#include "WME.h"

class Token : public std::vector<WME> {
public:
	void print(int level) const;
};

class TokenVector : public std::vector<Token> {
};