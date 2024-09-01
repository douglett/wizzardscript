#pragma once
#include "node.hpp"
#include <map>

namespace WizRun {
	using namespace std;
	int rcall(const string& funcname);
	int rsxpr(const Node& sx);

	ostream* output = &cout;

	Node program({});
	map<string, int> mem;

	int error(const string& msg) {
		throw runtime_error(msg);
	}

	// reset memory state
	int reset() {
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

	// statement commands
	int rprint(const Node& stmt) {
		for (size_t i = 1; i < stmt.list.size(); i++) {
			*output << (i > 1 ? " " : "");
			if    (stmt[i].type == Node::T_STRING)  cout << stmt[i].str;
			else  cout << rsxpr( stmt[i] );
		}
		*output << endl;
		return 0;
	}
	// int rblock() 

	int rsxpr(const Node& sx) {
		if (sx.type == Node::T_NUMBER)  return sx.num;
		if (!sx.issx())  return error("expected expression: " + sx.tostr());
		auto& type = sx[0].str;

		// statements
		if (type == "block") {
			for (size_t i = 1; i < sx.list.size(); i++)
				rsxpr( sx[i] );
			return 0;
		}
		else if (type == "print")  return rprint(sx);
		// memory
		else if (type == "get_global")  return mem[ sx[1].str ];
		// expressions
		// unknown
		else  return error("unexpected expression: " + sx.tostr());
		return 0;
	}

	int rcall(const string& funcname) {
		for (const auto& func : program.list)
			if (func.issx("function") && func[1].str == funcname)
				return rsxpr( func.findsx("block") );
		return error("missing function: " + funcname);
	}
}