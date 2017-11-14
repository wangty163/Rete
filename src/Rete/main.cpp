#include <iostream>

#include "ReteNet/Net.h"
#include "TestNode/TestAtTokenFilterNode.h"
#include "../Log.h"
#include "../StringOp.h"
#include "Utils/VectorPrinter.h"
#include "../COOVRecognitionByPattern.h"
#include "../PublicDefine.h"

void test() {
	CLog::write("Init", "start");
	COOVRecognitionByPattern::Init();
	CLog::write("Init", "end");

	CLog::write("Recog", "start");
	COOVRecognitionByPattern rc;
	std::vector<MatchResultNode> result;
	std::vector<std::string> words = {
		"��", "��", "��", "��", "��", "ֵ", "��", "�Ƿ�", "Ҫ", "ȥ��", "��", "��", "��", "û",
		"��", "��", "��", "��", "��"
	}, poses = {
		"m", "q", "n", "d", "v", "n", "w", "v", "v", "v", "r", "m", "q", "d", "v", "uj", "n", "ng", "w"
	};
	for (size_t i = 0; i < 100; ++i) {
		rc.RecognitionByOOVPattern(result, words, poses);
	}
	CLog::write("Recog", "end");
	for (auto&& nd : result) {
		debugIII(nd.nBegin, nd.nEnd, nd.matchedPattern);
	}
	system("pause");
}

int main() {
	test();
	return 0;
}
