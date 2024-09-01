#pragma once
#include "node.hpp"

namespace WizRun {
	using namespace std;
	int rcall(const string& funcname);

	ostream* output = &cout;

	Node program({});

	int reset() {
		output = &cout;
		program = Node({});
		return 0;
	}

	int run(const Node& prog) {
		program = prog;
		auto classname = program.findsx("info").findsx("mainclass")[1].str;
		// run static-init
		// rcall(classname + "__static_init");
		// run main
		rcall(classname + "__main");
		return 0;
	}

	int rprint(const Node& stmt) {
		for (size_t i = 1; i < stmt.list.size(); i++)
			*output << (i > 1 ? " " : "") << stmt[i].num;
		*output << endl;
		return 0;
	}

	int rblock(const Node& block) {
		for (size_t i = 1; i < block.list.size(); i++) {
			auto& stmt = block.list[i];
			if      (stmt.issx("print"))  rprint(stmt);
			else    throw runtime_error("unknown statement in block: " + stmt.str);
		}
		return 0;
	}

	int rcall(const string& funcname) {
		for (const auto& func : program.list)
			if (func.issx("function") && func[1].str == funcname) {
				auto& block = func[3];
				return rblock(block);
			}
		throw runtime_error("missing function: " + funcname);
	}
}