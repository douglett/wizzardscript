#pragma once
#include "node.hpp"
#include <map>

namespace WizRun {
	using namespace std;

	struct wizrun_ctrl_return {
		int rval = 0;
		wizrun_ctrl_return(int value) : rval(value) {}
	};
	struct HeapObject { string type; vector<int> data; };

	int reset();
	string mainclass();
	int rcall(const string& funcname);
	HeapObject& rderef(int ptr);
	string heaptostring(int ptr);
	static int rsxpr(const Node& sx);

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

	// helpful functions
	static int error(const string& msg) {
		throw runtime_error(msg);
	}

	// statement commands
	int rcall(const string& funcname) {
		for (const auto& func : program.list)
			if (func.issx("function") && func[1].str == funcname)
				try {
					return rsxpr( func.findsx("block") );
				}
				catch (wizrun_ctrl_return& ctrl) {
					return ctrl.rval;
				}
		return error("missing function: " + funcname);
	}
	static int rblock(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i++)
			sx[i].aslist(),
			rsxpr( sx[i] );
		return 0;
	}
	static int rprint(const Node& sx) {
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
	static int rinput(const Node& sx) {
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
	static int rif(const Node& sx) {
		for (size_t i = 1; i < sx.list.size(); i += 2)
			if (rsxpr( sx[i] )) 
				return rsxpr( sx[i+1] );
		return 0;
	}
	static int rwhile(const Node& sx) {
		while ( rsxpr(sx[1]) ) 
			rsxpr( sx[2] );
		return 0;
	}

	// heap memory
	string heaptostring(int ptr) {
		string s;
		for (auto i : rderef(ptr).data)
			s.push_back(i);
		return s;
	}
	HeapObject& rderef(int ptr) {
		if (ptr <= 0 || !heap.count(ptr))
			error("memory dereference error: " + to_string(ptr));
		return heap[ptr];
	}
	int& rderefat(int ptr, int pos) {
		auto& mem = rderef(ptr);
		if (pos < 0 || pos >= (int)mem.data.size())
			error("memory dereference bounds error: " + to_string(ptr) + " (at " + to_string(pos) + ")");
		return mem.data[pos];
	}
	static int rmake(const Node& sx) {
		auto& type = sx[1].str;
		int size = rsxpr(sx[2]);
		printf("make: %s %d :: %s\n", type.c_str(), size, sx[3].tostr().c_str());
		// fill with number
		if (sx[3].type == Node::T_NUMBER) 
			heap[++heap_top] = { type, vector<int>(size, sx[3].num) };
		// run constructor on each array member
		else {
			auto& mem = heap[++heap_top] = { type };
			for (int i = 0; i < size; i++)
				mem.data.push_back( rsxpr(sx[3]) );
		}
		return heap_top;
	}
	static int rstrcopy(const Node& sx) {
		int ptr = rsxpr(sx[1]);
		auto& mem = rderef(ptr);
		mem = {};
		for (auto c : stripliteral( sx[2].str ))
			mem.data.push_back(c);
		return ptr;
	}

	static int rsxpr(const Node& sx) {
		// basic types
		if      (sx.type == Node::T_NUMBER)  return sx.num;
		else if (sx.str == "true")           return 1;
		else if (sx.str == "false")          return 0;
		// ensure s-expression
		else if (!sx.issx())                 return error("expected expression: " + sx.tostr());

		auto& type = sx[0].str;
		// statements
		if      (type == "block")   return rblock(sx);
		else if (type == "print")   return rprint(sx);
		else if (type == "input")   return rinput(sx);
		else if (type == "if")      return rif(sx);
		else if (type == "while")   return rwhile(sx);
		else if (type == "call")    return rcall( mainclass() + "__" + sx[1].str );
		else if (type == "return")  throw wizrun_ctrl_return( rsxpr(sx[1]) );
		// memory
		else if (type == "get_global")   return mem[ sx[1].str ];
		else if (type == "set_global")   return mem[ sx[1].str ] = rsxpr( sx[2] );
		else if (type == "make")         return rmake(sx);
		else if (type == "get_offset")   return rderefat( rsxpr(sx[1]), rsxpr(sx[2]) );
		else if (type == "set_offset")   return rderefat( rsxpr(sx[1]), rsxpr(sx[2]) ) = rsxpr(sx[3]);
		else if (type == "string_copy")  return rstrcopy(sx);
		// expressions
		else if (type == "||")  return rsxpr(sx[1]) || rsxpr(sx[2]);
		else if (type == "&&")  return rsxpr(sx[1]) && rsxpr(sx[2]);
		else if (type == "==")  return rsxpr(sx[1]) == rsxpr(sx[2]);
		else if (type == "!=")  return rsxpr(sx[1]) != rsxpr(sx[2]);
		else if (type == ">=")  return rsxpr(sx[1]) >= rsxpr(sx[2]);
		else if (type == "<=")  return rsxpr(sx[1]) <= rsxpr(sx[2]);
		else if (type == ">" )  return rsxpr(sx[1]) >  rsxpr(sx[2]);
		else if (type == "<" )  return rsxpr(sx[1]) <  rsxpr(sx[2]);
		else if (type == "+" )  return rsxpr(sx[1]) +  rsxpr(sx[2]);
		else if (type == "-" )  return rsxpr(sx[1]) -  rsxpr(sx[2]);
		else if (type == "*" )  return rsxpr(sx[1]) *  rsxpr(sx[2]);
		else if (type == "/" )  return rsxpr(sx[1]) /  rsxpr(sx[2]);
		// unknown
		else    return error("unexpected expression: " + sx.tostr());
	}
}