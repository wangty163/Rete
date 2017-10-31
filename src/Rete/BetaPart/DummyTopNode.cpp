#include "DummyTopNode.h"

DummyTopNode::DummyTopNode()
	: ReteNode(true) {
	output.push_back({});
}

void DummyTopNode::clearStatus() {
}

const TokenVector & DummyTopNode::getOutput() {
	return output;
}
