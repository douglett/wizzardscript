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
		auto& static_init = program.push({ "function", classmember("static_init"), {} });
		auto& static_init_block = static_init.push({ "block" });
		// block or flat class
		int isblock = accept("{");
		if (!isblock)  require(";");
		// class contents
		while (true)
			// if      ( peek("}") || peek("$EOF") )  break;
			if      ( pfunction(static_init_block) )  ;
			// else if ( pdim(static_init_block) )  ;
			else    break;
		// end class
		if (isblock)  require("}");
		require("$EOF");
		trace("end class: " + classname);
		classname = "";
		return 1;
	}

	int pfunction(Node& parent) {
		if (!accept("$type $identifier ("))  return 0;
		auto type = presults[0];
		auto name = presults[1];
		trace("begin function: " + type + " " + name);
		auto& func = parent.push({ "function", classmember(name), {} });
		// TODO: parse function arguments
		require(")");
		// pblock(func, "function");
		return 1;
	}
}