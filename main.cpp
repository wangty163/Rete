#include <iostream>

#include "Net.h"

using namespace std;

//debug
#define _debug
#ifdef _debug
#define dt(a)  << (#a) << "=" << (a) << " "
#define debugI(a) std::cout dt(a) << std::endl
#define debugII(a, b) std::cout dt(a) dt(b) << std::endl
#define debugIII(a, b, c) std::cout dt(a) dt(b) dt(c) << std::endl
#define debugIV(a, b, c, d) std::cout dt(a) dt(b) dt(c) dt(d) << std::endl
#define debugV(a, b, c, d, e) std::cout dt(a) dt(b) dt(c) dt(d) dt(e) << std::endl
#else
#define debugI(v)
#define debugII(a, b)
#define debugIII(a, b, c)
#define debugIV(a, b, c, d)
#define debugV(a, b, c, d, e)
#endif

void addSentence(Net& net, std::vector<std::string> wordVector, 
	std::vector<std::string> posVector) {

	myAssert(wordVector.size() == posVector.size());
	for (size_t i = 0; i < wordVector.size(); ++i) {
		net.addWME({ i, "word", wordVector.at(i) });
		net.addWME({ i, "len", wordVector.at(i).length() / 2 });
		net.addWME({ i, "pos", posVector.at(i) });
		net.addWME({ i, "offset-start", i });
		net.addWME({ i, "offset-end", i + 1 });
	}
}

void test_1() {
	Net net;
	net.addProduction({
		{ "$w1", "pos", "p" },
		{ "$w1", "offset-end", "$t2" },
		{ "$w2", "offset-start", "$t2" },
		{ "$w1", "len", "$t1" },
		{ "$t1", "numeric-greater-than", "1", Condition::negetive },
		{ "$w2", "pos", "p" },
	});
	std::vector<std::string> words = {
		"由",
		"总统",
		"亲自",
		"裁剪",
	};
	std::vector<std::string> poses = {
		"p",
		"p",
		"p",
		"v",
	};
	addSentence(net, words, poses);
	net.invoke();
}

void test_2() {
	Net net;
	net.addProduction({
		{ "$x", "on", "$y" },
		{ "$y", "left-of", "$z" },
		{ "$z", "color", "red" },
	}); 
	
	net.addWME({ "B1", "on", "B2" });
	net.addWME({ "B1", "on", "B3" });
	net.addWME({ "B1", "color", "red" });
	net.addWME({ "B2", "on", "table" });
	net.addWME({ "B2", "left-of", "B3" });
	net.addWME({ "B2", "color", "blue" });
	net.addWME({ "B3", "left-of", "B4" });
	net.addWME({ "B3", "on", "table" });
	net.addWME({ "B3", "color", "red" });

	net.invoke();
}

int main() {
	test_2();
	return 0;
}
