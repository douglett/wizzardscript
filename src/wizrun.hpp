#pragma once
#include "node.hpp"

namespace WizRun {
	using namespace std;
	int rcall(const string& funcname);

	Node program({});

	int run(const Node& prog) {
		program = prog;
		auto classname = program.findsx("info").findsx("mainclass")[1].str;
		// run static-init
		rcall(classname + "__static_init");
		// run main
		return 0;
	}

	int rcall(const string& funcname) {
		auto& func = program.findsx(funcname);
		auto& block = func[3];
		// rblock(block);
		return 0;
	}
}