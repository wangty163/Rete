#pragma once

#include "../../HashSupport.h"

template <typename O>
class Node {
	static size_t count;
protected:
	O output;
public:
	size_t serialNumber;
	Node();
	const O& getOutput();
	virtual void clearStatus() = 0;

	bool operator==(const Node<O>& rhs) const;
	size_t hashCode() const;
};

template <typename O>
size_t Node<O>::count = 0;

// 注意：非线程安全
template<typename O>
inline Node<O>::Node() : serialNumber(count++) {
}

template<typename O>
inline const O & Node<O>::getOutput() {
	return output;
}

template<typename O>
inline bool Node<O>::operator==(const Node<O>& rhs) const {
	return serialNumber == rhs.serialNumber;
}

template<typename O>
inline size_t Node<O>::hashCode() const {
	return std::hash<size_t>()(serialNumber);
}

namespace std {
	template<typename O>
	struct hash<Node<O>> {
		size_t operator()(const Node<O>& node) const {
			return node.hashCode();
		}
	};
}