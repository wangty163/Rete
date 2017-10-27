#pragma once

#include "WME.h"

class Token : public std::vector<WME> {
public:
	void print(size_t level) const;
};

class TokenVector : public std::vector<Token> {
};