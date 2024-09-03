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

	int reset() {
		program = {};
		mem = {};
		return 0;
	}

	int run(const Node& prog) {
		// reset program state
		reset();
		program = prog;
		// initialize and run class
		auto classname = program.findsx("info").findsx("mainclass")[1].str;
		// rcall(classname + "__static_init");
		rcall(classname + "__main");
		return 0;
	}

	// statement commands
	int rprint(const Node& stmt) {
		for (size_t i = 1; i < stmt.list.size(); i++) {
			*output << (i > 1 ? " " : "");
			if    (stmt[i].type == Node::T_STRING)  *output << stmt[i].str;
			else  *output << rsxpr( stmt[i] );
		}
		*output << endl;
		return 0;
	}
	int rblock(const Node& block) {
		for (size_t i = 1; i < block.list.size(); i++)
			block[i].aslist(),
			rsxpr( block[i] );
		return 0;
	}

	int rsxpr(const Node& sx) {
		if (sx.type == Node::T_NUMBER)  return sx.num;
		if (!sx.issx())  return error("expected expression: " + sx.tostr());
		auto& type = sx[0].str;

		// statements
		if (type == "block")  return rblock(sx);
		else if (type == "print")  return rprint(sx);
		// memory
		else if (type == "get_global")  return mem[ sx[1].str ];
		else if (type == "set_global")  return mem[ sx[1].str ] = rsxpr( sx[2] );
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