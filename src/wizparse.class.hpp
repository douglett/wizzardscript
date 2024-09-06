#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	static int pfunction(Node& parent);
	static int pblock(Node& parent, const string& name);
	static int pdim(Node& parent);

	static string classname;

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

	static int pdim(Node& parent) {
		if (!accept("$type $identifier"))  return false;
		auto type = presults[0], name = presults[1];
		scope_dim(type, name);
		// dim by type
		if (type == "int") {
			auto& stmt = parent.push({ "set_global", classmember(name), 0 });
			if (accept("=")) {
				stmt.pop();
				pexpras(stmt, "int");
			}
		}
		else if (type == "string") {
			parent.push({ "set_global", classmember(name), { "make", "string", 0 } });
			if (accept("=")) {
				auto& stmt = parent.push({ "string_copy", { "get_global", classmember(name) } });
				pexpras(stmt, "string");
			}
		}
		else
			error("unexpected type: " + type);
		require(";");
		return true;
	}

	static int pset(Node& parent) {
		if (!accept("$identifier ="))  return false;
		auto name = presults[0];
		auto& dim = scope_find(name);
		// set by type
		if (dim.type == "int") {
			auto& stmt = parent.push({ "set_global", classmember(name) });
			pexpras(stmt, "int");
		}
		else if (dim.type == "string") {
			auto& stmt = parent.push({ "string_copy", { "get_global", classmember(name) } });
			pexpras(stmt, "string");
		}
		else
			error("unexpected type in set: " + dim.type);
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
		// else condition
		if (accept("else")) {
			stmt.push("true");
			pblock(stmt, "if-else");
		}
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
				|| pset(block)
				|| pinput(block)
				|| pif(block)
				|| error_unexpected();
		}
		// block end
		require("}");
		trace("end block: " + name);
		return true;
	}

	static int pfunction(Node& parent) {
		if (!accept("$type $identifier ("))  return false;
		auto type = presults[0];
		auto name = presults[1];
		trace("begin function: " + type + " " + name);
		auto& func = parent.push({ "function", classmember(name), {} });
		// TODO: parse function arguments
		require(")");
		pblock(func, "function");
		// pblock(func, "function") || error_expected("block");
		return true;
	}
}