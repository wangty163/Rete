#pragma once

#include "WME.h"
#include "../../HashSupport.h"

class Token : public std::vector<WME> {
public:
	void print(int level) const;

	size_t hashCode() const;
};

DEFINE_STD_HASH_SPECIALIZATION(Token);

class TokenSet : public std::unordered_set<Token> {
};