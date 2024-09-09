#pragma once
#include "node.hpp"
#include <map>

namespace WizRun {
	using namespace std;
	int reset();
	string mainclass();

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

	int run(const Node& prog) {
		// reset program state
		reset();
		program = prog;
		// initialize and run class
		auto classname = mainclass();
		rcall(classname + "__static_init");
		rcall(classname + "__main");
		return 0;
	}

	int reset() {
		program = {};
		mem = {};
		heap = {};
		heap_top = 0;
		return 0;
	}

	string mainclass() {
		return program.findsx("info").findsx("mainclass")[1].str;
	}

	string heaptostring(int ptr) {
		string s;
		for (auto i : rderef(ptr).data)
			s.push_back(i);
		return s;
	}

	int error(const string& msg) {
		throw runtime_error(msg);
	}

	// statement commands
	int rblock(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i++)
			sx[i].aslist(),
			rsxpr( sx[i] );
		return 0;
	}
	int rprint(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i++) {
			auto arg = sx[i];
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
	int rinput(const Node& sx) {
		int ptr = rsxpr(sx[2]);
		auto& mem = rderef(ptr);
		string s;
		*output << stripliteral( sx[1].str );  // prompt
		getline(*input, s);
		mem.data = {};
		for (auto c : s)
			mem.data.push_back(c);
		return 0;
	}
	int rif(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i += 2)
			if (rsxpr( sx[i] )) 
				return rsxpr( sx[i+1] );
		return 0;
	}
	int rwhile(const Node& sx) {
		while ( rsxpr(sx[1]) ) 
			rsxpr( sx[2] );
		return 0;
	}
	int ror(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i++)
			if ( rsxpr(sx[i]) )
				return true;
		return false;
	}
	int rand(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i++)
			if ( !rsxpr(sx[i]) )
				return false;
		return true;
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
		// basic types
		if      (sx.type == Node::T_NUMBER)  return sx.num;
		else if (sx.str == "true")  return 1;
		else if (sx.str == "false")  return 0;
		// ensure s-expression
		if (!sx.issx())  return error("expected expression: " + sx.tostr());
		auto& type = sx[0].str;

		// statements
		if      (type == "block")  return rblock(sx);
		else if (type == "print")  return rprint(sx);
		else if (type == "input")  return rinput(sx);
		else if (type == "if")  return rif(sx);
		else if (type == "while")  return rwhile(sx);
		else if (type == "call")  return rcall( mainclass() + "__" + sx[1].str );
		// memory
		else if (type == "get_global")  return mem[ sx[1].str ];
		else if (type == "set_global")  return mem[ sx[1].str ] = rsxpr( sx[2] );
		else if (type == "make")  return rmake(sx);
		else if (type == "string_copy")  return rstrcopy(sx);
		// expressions
		else if (type == "||")  return ror(sx);
		else if (type == "&&")  return rand(sx);
		else if (type == "==")  return rsxpr(sx[1]) == rsxpr(sx[2]);
		else if (type == "!=")  return rsxpr(sx[1]) != rsxpr(sx[2]);
		else if (type == ">=")  return rsxpr(sx[1]) >= rsxpr(sx[2]);
		else if (type == "<=")  return rsxpr(sx[1]) <= rsxpr(sx[2]);
		else if (type == ">" )  return rsxpr(sx[1]) >  rsxpr(sx[2]);
		else if (type == "<" )  return rsxpr(sx[1]) <  rsxpr(sx[2]);
		else if (type == "+" )  return rsxpr(sx[1]) +  rsxpr(sx[2]);
		// unknown
		else    return error("unexpected expression: " + sx.tostr());
	}

	int rcall(const string& funcname) {
		for (const auto& func : program.list)
			if (func.issx("function") && func[1].str == funcname)
				return rsxpr( func.findsx("block") );
		return error("missing function: " + funcname);
	}
}