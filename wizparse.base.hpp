#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"
// #include "helpers.hpp"

namespace WizParse {
	using namespace std;

// >> state
	Tokenizer tok;
	const vector<string> keywords = {
		"static", "class",
		"print"
		// "if", "else", "while", "return",
		// "push", "pop", "concat", "clear",
		// "int", "float", "bool", "string"
	};

// >> error handling
	class parse_error : public runtime_error {
		using runtime_error::runtime_error;
	};
	int error(const string& s) {
		throw parse_error(s);
	}

// >> basic token parsing
	int accept(const string& rulesstr) {
		const int pos = tok.pos;
		const auto rules = splitstr(rulesstr);
		vector<string> results;
		return 0;

		// parse rules
		// for (const auto& rule : rules) {
		// 	if (rule == "$identifier" && isidentifier(tok.peek()))
		// 		results.push(tok.get());
		// 	else if (rule == tok.peek())
		// 		results.push(tok.get());
		// 	else
		// 		return tok.pos = pos, results = {}, 0;
		// }
		// // all found
		// return 1;
	}
	// int peek(const string& rulesstr) {
	int require(const string& rulesstr) {
		if (!accept(rulesstr))
			error("missing rule: \"" + rulesstr + "\"");
		return 1;
	}
	
// >> class structure
	int pclass() {
		require("static class $identifier ;");
		return 0;
	}
};