#include <iostream>

#include "ReteNet/Net.h"
#include "TestNode/TestAtTokenFilterNode.h"
#include "../Log.h"
#include "../StringOp.h"
#include "Utils/VectorPrinter.h"
#include "../PublicDefine.h"

void test() {
	Net net;
	net.addProduction({
		// production
		{ "$x", ".color", "red" },
	}, {
		// display when match
		{ "rule 1", "", ""},
		{ "$x", "color", "red" },
	});
	net.addProduction({
		// production
		{ "$x", ".on", "$y" },
		{ "$y", ".left-of", "$z" },
		{ "$z", ".color", "red" },
	}, {
		// display when match
		{ "rule 2", "", ""},
		{ "$x", "on", "$y" },
		{ "$y", "left-of", "$z" },
		{ "$z", "color", "red" },
	});

	net.addWME({ "B1", ".on", "B2" });
	net.addWME({ "B1", ".on", "B3" });
	net.addWME({ "B1", ".color", "red" });
	net.addWME({ "B2", ".on", "table" });
	net.addWME({ "B2", ".left-of", "B3" });
	net.addWME({ "B2", ".color", "blue" });
	net.addWME({ "B3", ".left-of", "B4" });
	net.addWME({ "B3", ".on", "table" });
	net.addWME({ "B3", ".color", "red" });

	;
	for (auto&& condVec : net.invoke()) {
		for (auto&& cond : condVec) {
			cond.print(0);
		}
		puts("");
	}
	system("pause");
}

int main() {
	test();
	return 0;
}
