#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"
// #include "helpers.hpp"

namespace WizParse {
	using namespace std;
	typedef  vector<string>  vecs;

// >> state
	Tokenizer tok;
	const vector<string> keywords = {
		"static", "class",
		"print"
		// "if", "else", "while", "return",
		// "push", "pop", "concat", "clear",
		// "int", "float", "bool", "string"
	};
	vecs presults;

// >> error handling
	class parse_error : public runtime_error {
		using runtime_error::runtime_error;
	};
	int error(const string& s) {
		throw parse_error(s);
	}
	void trace(const string& s) {
		cout << "> " << s << endl;
	}

// >> basic token parsing
	int accept(const string& rulesstr, vecs& results = presults) {
		const int pos = tok.pos;
		const auto rules = splitstr(rulesstr);
		results = {};
		// parse rules
		for (const auto& rule : rules) {
			if (rule == "$identifier" && isidentifier(tok.peek()))
				results.push_back(tok.get());
			else if (rule == tok.peek())
				results.push_back(tok.get());
			else
				return tok.pos = pos, results = {}, 0;
		}
		// all found
		return 1;
	}
	int peek(const string& rulesstr, vecs& results = presults) {
		const int pos = tok.pos;
		if (!accept(rulesstr, results))  return tok.pos = pos, 0;
		return 1;
	}
	int require(const string& rulesstr, vecs& results = presults) {
		if (!accept(rulesstr, results))
			error("missing rule: \"" + rulesstr + "\"");
		return 1;
	}
	
// >> class structure
	int pclass() {
		// vecs results;
		require("static class $identifier ;");
		auto classname = presults[2];
		trace("begin class: " + classname);
		trace("end class: " + classname);
		return 1;
	}
};