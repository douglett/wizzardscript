#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	static int pfunction(Node& parent);
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

	// TODO: proper expression parsing
	static int pexpr(Node& parent) {
		string type;
		if (pvarpath(parent, type))
			return (type == "int" || error_expected("variable-as-number")), true;
		else if (accept("$number"))
			return parent.push( stoi(presults[0]) ), true;
		return error_expected("expression");
	}

	// TODO: implement
	static int pstrexpr(Node& parent) {
		int pos = tok.pos;
		string type;
		if (pvarpath(parent, type))
			return type == "string" ? true : ( tok.pos = pos, parent.pop(), false );
		else if (accept("$literal"))
			return parent.push( presults[0].c_str() ), true;
		return false;
	}

	static int pprint(Node& parent) {
		if (!accept("print"))  return false;
		auto& stmt = parent.push({ "print" });
		Node temp({});
		do
			if (pstrexpr(temp))
				stmt.push({ "string", temp.pop() });
			else
				pexpr(stmt);
		while (accept(","));
		require(";");
		return true;
	}

	static int pdim(Node& parent) {
		if (!accept("$type $identifier"))  return false;
		auto type = presults[0], name = presults[1];
		scope_dim(type, name);
		if (type == "int") {
			auto& stmt = parent.push({ "set_global", classmember(name) });
			if    (accept("="))  pexpr(stmt);
			else  stmt.push(0);
		}
		else if (type == "string") {
			parent.push({ "set_global", classmember(name), { "make", "string", 0 } });
		}
		else
			error("unexpected type: " + type);
		require(";");
		return true;
	}

	static int pset(Node& parent) {
		if (!accept("$identifier ="))  return false;
		auto name = presults[0];
		auto& stmt = parent.push({ "set_global", classmember(name) });
		pexpr(stmt);
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
				// || pdim(block)
				|| pset(block)
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