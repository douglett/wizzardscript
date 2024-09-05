#pragma once
#include "node.hpp"
#include <map>

namespace WizRun {
	using namespace std;
	struct HeapObject { string type; vector<int> data; };

	int rcall(const string& funcname);
	int rsxpr(const Node& sx);
	HeapObject& rderef(int ptr);

	ostream* output = &cout;
	istream* input = &cin;
	Node program({});
	map<string, int> mem;
	map<int, HeapObject> heap;
	int heap_top = 0;

	int error(const string& msg) {
		throw runtime_error(msg);
	}

	int reset() {
		program = {};
		mem = {};
		heap = {};
		heap_top = 0;
		return 0;
	}

	string heaptostring(int ptr) {
		string s;
		for (auto i : rderef(ptr).data)
			s.push_back(i);
		return s;
	}

	int run(const Node& prog) {
		// reset program state
		reset();
		program = prog;
		// initialize and run class
		auto classname = program.findsx("info").findsx("mainclass")[1].str;
		rcall(classname + "__static_init");
		rcall(classname + "__main");
		return 0;
	}

	// statement commands
	int rblock(const Node& block) {
		for (size_t i = 1; i < block.list.size(); i++)
			block[i].aslist(),
			rsxpr( block[i] );
		return 0;
	}
	int rprint(const Node& stmt) {
		for (size_t i = 1; i < stmt.list.size(); i++) {
			auto arg = stmt[i];
			*output << (i > 1 ? " " : "");
			if (arg.type == Node::T_STRING)
				*output << stripliteral( arg.str );
			else if (arg.issx("string"))
				*output << heaptostring( rsxpr(arg[1]) );
			else
				*output << rsxpr(arg);
		}
		*output << endl;
		return 0;
	}
	int rinput(const Node& stmt) {
		int ptr = rsxpr(stmt[2]);
		auto& mem = rderef(ptr);
		string s;
		*output << stripliteral( stmt[1].str );  // prompt
		getline(*input, s);
		mem.data = {};
		for (auto c : s)
			mem.data.push_back(c);
		return 0;
	}

	// heap memory
	HeapObject& rderef(int ptr) {
		if (!heap.count(ptr))
			error("memory dereference error: " + to_string(ptr));
		return heap[ptr];
	}
	int rmake(const Node& sx) {
		auto& type = sx[1].str;
		int size = rsxpr(sx[2]);
		heap[++heap_top] = { type, vector<int>(size, 0) };
		return heap_top;
	}
	int rstrcopy(const Node& sx) {
		int ptr = rsxpr(sx[1]);
		auto& mem = rderef(ptr);
		mem = {};
		for (auto c : stripliteral( sx[2].str ))
			mem.data.push_back(c);
		return ptr;
	}

	int rsxpr(const Node& sx) {
		if (sx.type == Node::T_NUMBER)  return sx.num;
		if (!sx.issx())  return error("expected expression: " + sx.tostr());
		auto& type = sx[0].str;

		// statements
		if (type == "block")  return rblock(sx);
		else if (type == "print")  return rprint(sx);
		else if (type == "input")  return rinput(sx);
		// memory
		else if (type == "get_global")  return mem[ sx[1].str ];
		else if (type == "set_global")  return mem[ sx[1].str ] = rsxpr( sx[2] );
		else if (type == "make")  return rmake(sx);
		else if (type == "string_copy")  return rstrcopy(sx);
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