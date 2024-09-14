#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	static int predefine();
	static int pfunctiondef(string& name, bool predef);
	static int pfunction(Node& parent);
	static int pblock(Node& parent, const string& name);
	static int pdim(Node& parent);

	static string classname;
	static string functype;

	const char* classmember(const string& name) {
		static string s;
		return (s = classname + "__" + name).c_str();
	}

	int pclass() {
		require("static class $identifier");
		classname = presults[2];
		trace("begin class: " + classname);
		auto& static_init = program.push({ "function", classmember("static_init"), {} });
		auto& static_init_block = static_init.push({ "block" });
		// set as main class
		program.findsx("info").findsx("mainclass").push(classname.c_str());
		// block or flat class
		int isblock = accept("{");
		if (!isblock)  require(";");
		// predefine functions and variables before full parsing
		predefine();
		// class contents
		while (true)
			if      ( tok.peek() == "}" || tok.eof() )  break;
			else if ( pfunction(program) )  ;
			else if ( pdim(static_init_block) )  ;
			else    error_unexpected();
		// end class
		if (isblock)  require("}");
		require("$EOF");
		classname = "";
		return true;
	}

	static int predefine() {
		int pos = tok.pos;
		string type, name;
		Node ntemp({});
		while (!tok.eof())
			if (pfunctiondef(name, true)) {
				require("{");
				int brackets = 1;
				while (!tok.eof()) {
					if      (tok.peek() == "{")  brackets++;
					else if (tok.peek() == "}")  brackets--;
					if      (brackets <= 0)  break;
					tok.get();
				}
				require("}");
			}
			else if (accept("$type $identifier")) {
				type = presults[0], name = presults[1];
				if (accept("[")) {
					pexpras(ntemp, "int");
					require("]");
					type += "[]";
				}
				scope_dim(type, name);
				// walk to end-of-line
				while (!tok.eof() && tok.peek() != ";")
					tok.get();
				require(";");
			}
			else
				tok.get();
		tok.pos = pos;
		return true;
	}

	static int pfunctiondef(string& name, bool predef) {
		if (!accept("$type $identifier ("))  return false;
		FnDef def = { presults[0], presults[1] };
		name = def.name;
		trace("begin function def: " + def.type + " " + def.name);
		// TODO: alternate return types
		if (def.type != "int")
			error("function: only int type supported: " + def.type);
		// arguments
		if (accept("$type $identifier")) {
			def.args.push_back({ presults[0], presults[1] });
		}
		require(")");
		// save definition
		if (predef)
			func_def(def);
		trace("end function def");
		return true;
	}

	static int pfunction(Node& parent) {
		string name;
		if (!pfunctiondef(name, false))  return false;
		auto& def = func_find(name);
		functype = def.type;
		// build function
		auto& func = parent.push({ "function", classmember(def.name) });
		auto& args = func.push({});
		for (const auto& dim : def.args) {
			args.push(dim.name.c_str());
			// TODO: scope args
		}
		// parse block
		pblock(func, "function");
		functype = "";
		return true;
	}

	static int pdim(Node& parent) {
		if (!accept("$type $identifier"))  return false;
		auto type = presults[0], name = presults[1];
		// scope_dim(type, name);
		// dim by type
		if (type == "int") {
			if (accept("[")) {
				auto& stmt = parent.push({ "set_global", classmember(name) });
				auto& make = stmt.push({ "make", "int[]" });
				pexpras(make, "int");  // size
				make.push(0);  // default value
				require("]");
			}
			else if (accept("=")) {
				auto& stmt = parent.push({ "set_global", classmember(name) });
				pexpras(stmt, "int");
			}
			else {
				parent.push({ "set_global", classmember(name), 0 });
			}
		}
		else if (type == "string") {
			if (accept("[")) {
				auto& stmt = parent.push({ "set_global", classmember(name) });
				auto& make = stmt.push({ "make", "string[]" });
				pexpras(make, "int");  // size
				make.push({ "make", "string", 0, 0 });  // default value
				require("]");
			}
			else if (accept("=")) {
				parent.push({ "set_global", classmember(name), { "make", "string", 0, 0 } });
				auto& stmt = parent.push({ "string_copy", { "get_global", classmember(name) } });
				pexpras(stmt, "string");
			}
			else {
				parent.push({ "set_global", classmember(name), { "make", "string", 0, 0 } });
			}
		}
		else
			error("unexpected type: " + type);
		require(";");
		return true;
	}

	static int pprint(Node& parent) {
		if (!accept("print"))  return false;
		auto& stmt = parent.push({ "print" });
		string type;
		do {
			pexpr(stmt, type);
			if (type == "string" && stmt.list.back().type != Node::T_STRING)
				stmt.push({ "string", stmt.pop() });
		}
		while (accept(","));
		require(";");
		return true;
	}

	static int pinput(Node& parent) {
		if (!accept("input"))  return false;
		auto& stmt = parent.push({ "input", "\"\"" });
		if (accept("$literal ,"))
			stmt.list.back().str = presults[0];
		string type;
		if (!pvarpath(stmt, type) || type != "string")
			error_expected("expected string variable");
		require(";");
		return true;
	}

	static int pif(Node& parent) {
		// if condition
		if (!accept("if"))  return false;
		auto& stmt = parent.push({ "if" });
		require("("), pexpras(stmt, "int"), require(")");
		pblock(stmt, "if");
		// else-if condition
		while (accept("else if")) {
			require("("), pexpras(stmt, "int"), require(")");
			pblock(stmt, "else-if");
		}
		// else condition
		if (accept("else")) {
			stmt.push("true");
			pblock(stmt, "else");
		}
		return true;
	}

	static int pwhile(Node& parent) {
		if (!accept("while"))  return false;
		auto& stmt = parent.push({ "while" });
		require("("), pexpras(stmt, "int"), require(")");
		pblock(stmt, "while");
		return true;
	}

	static int pset(Node& parent) {
		// find variable path
		int pos = tok.pos;
		string type;
		if (!pvarpath(parent, type))  return false;
		if (!accept("="))  return parent.pop(), tok.pos = pos, false;
		auto& vpath = parent.list.back();
		// change get to set
		if (type == "int") {
			if      (vpath.issx("get_global"))  vpath.list[0].str = "set_global";
			else if (vpath.issx("get_offset"))  vpath.list[0].str = "set_offset";
			else    error("unexpected vpath type: " + vpath.sxtype());
			pexpras(vpath, "int");
		}
		else if (type == "string") {
			auto& stmt = parent.push({ "string_copy", parent.pop() });
			pexpras(stmt, "string");
		}
		else
			error("unexpected type in set: " + type);
		// OK
		require(";");
		return true;
	}

	static int preturn(Node& parent) {
		if (!accept("return"))  return false;
		auto& stmt = parent.push({ "return" });
		string type;
		if (pexpr(stmt, type)) {
			if (type != functype)
				error("return type mismatch: " + functype + ", " + type);
		}
		else
			stmt.push(0);
		require(";");
		return true;
	}

	static int pexprline(Node& parent) {
		string type;
		if (!pexpr(parent, type, false))  return false;
		require(";");
		return true;
	}

	static int pblock(Node& parent, const string& name) {
		// block start
		require("{");
		trace("begin block: " + name);
		auto& block = parent.push({ "block" });
		// statements
		while (true) {
			if (tok.eof() || tok.peek() == "}")  break;
			pprint(block)
				|| pinput(block)
				|| pif(block)
				|| pwhile(block)
				|| preturn(block)
				|| pset(block)
				|| pexprline(block)
				|| error_unexpected();
		}
		// block end
		require("}");
		trace("end block: " + name);
		return true;
	}
}