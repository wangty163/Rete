#include <iostream>

#include "ReteNet/Net.h"
#include "TestNode/TestAtTokenFilterNode.h"
#include "../Log.h"
#include "../StringOp.h"
#include "Utils/VectorPrinter.h"
#include "../COOVRecognitionByPattern.h"
#include "../PublicDefine.h"

void test() {
	COOVRecognitionByPattern::Init();

	COOVRecognitionByPattern rc;
	std::vector<MatchResultNode> result;
	rc.RecognitionByOOVPattern(result, {
		"几", "个", "月", "不", "冲", "值", "，", "是否", "要", "去掉", "那", "几", "个", "没", 
		"冲", "的", "月", "租", "？"
	}, {
		"m", "q", "n", "d", "v", "n", "w", "v", "v", "v", "r", "m", "q", "d", "v", "uj", "n", "ng", "w"
	});
	for (auto&& nd : result) {
		debugIII(nd.nBegin, nd.nEnd, nd.matchedPattern);
	}
	system("pause");
}

int main() {
	test();
	return 0;
}
