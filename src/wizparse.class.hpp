#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	int pfunction(Node& parent);

	string classname;

	const char* classmember(const string& name) {
		static string s;
		return (s = classname + "__" + name).c_str();
	}

	int pclass() {
		require("static class $identifier");
		classname = presults[2];
		trace("begin class: " + classname);
		// auto& static_init = program.push({ "function", classmember("static_init"), {} });
		// auto& static_init_block = static_init.push({ "block" });
		// set as main class
		program.findsx("info").findsx("mainclass").push(classname.c_str());
		// block or flat class
		int isblock = accept("{");
		if (!isblock)  require(";");
		// class contents
		while (true)
			if      ( pfunction(program) )  ;
			// else if ( pdim(static_init_block) )  ;
			else    break;
		// end class
		if (isblock)  require("}");
		require("$EOF");
		classname = "";
		return true;
	}

	// temporary 
	int pexpr(Node& parent) {
		string type;
		if (pvarpath(parent, type))
			return (type == "int" || error_expected("variable-as-number")), true;
		else if (accept("$number"))
			return parent.push( stoi(presults[0]) ), true;
		// return false;
		return error_expected("expression");
	}

	int pprint(Node& parent) {
		if (!accept("print"))  return false;
		auto& stmt = parent.push({ "print" });
		pexpr(stmt);
		while (accept(","))
			pexpr(stmt);
		require(";");
		return true;
	}

	int pset(Node& parent) {
		if (!accept("$identifier ="))  return false;
		auto name = presults[0];
		auto& stmt = parent.push({ "set_global", classmember(name) });
		pexpr(stmt);
		require(";");
		return true;
	}

	int pblock(Node& parent, const string& name) {
		// block start
		require("{");
		trace("begin block: " + name);
		auto& block = parent.push({ "block" });
		// statements
		while (true) {
			if (tok.eof() || tok.peek() == "}")  break;
			pprint(block)
				|| pset(block)
				|| error_unexpected();
		}
		// block end
		require("}");
		trace("end block: " + name);
		return true;
	}

	int pfunction(Node& parent) {
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