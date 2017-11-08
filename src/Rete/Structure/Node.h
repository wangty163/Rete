#pragma once

template <typename O>
class Node {
	static size_t count;
protected:
	O output;
public:
	size_t serialNumber;
	Node();
	virtual const O& getOutput() = 0;
	virtual void clearStatus() = 0;

	template<typename X>
	bool operator==(const Node<X>& rhs) const;
};

template <typename O>
size_t Node<O>::count = 0;

// 注意：非线程安全
template<typename O>
inline Node<O>::Node() : serialNumber(count++) {
}

template<typename O>
template<typename X>
inline bool Node<O>::operator==(const Node<X>& rhs) const {
	return serialNumber == rhs.serialNumber;
}
