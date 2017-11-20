#include "Agent.h"

void Agent::addToOutput(const Token & token, const ConditionVector & conditionVector) {
	dict[token].push_back(conditionVector);
}

void Agent::removeFromOutput(const Token & token) {
	dict.erase(token);
}

void Agent::clearStatus() {
	dict.clear();
}

std::vector<ConditionVector> Agent::getOutput() const {
	std::vector<ConditionVector> ret;
	for (auto&& pair : dict) {
		for (auto&& conditionVector : pair.second)
			ret.push_back(conditionVector);
	}
	return ret;
}
