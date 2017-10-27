#pragma once

template <typename O>
class Node {
protected:
	O output;
public:
	virtual const O& getOutput() = 0;
};