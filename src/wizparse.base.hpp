#pragma once
#include <string>
#include <vector>
#include <cassert>
#include "tokenizer.hpp"
#include "helpers.hpp"
#include "node.hpp"

namespace WizParse {
	using namespace std;

	// >> definitions
	typedef  vector<string>  vecs;
	class parse_error : public runtime_error {
		using runtime_error::runtime_error;
	};
	const vector<string> keywords = {
		"static", "class",
		"print"
		// "if", "else", "while", "return",
		// "push", "pop", "concat", "clear",
	};
	const vector<string> basetypes = { "int" };

	// >> forward-define
	int istype(const string& str);
	// .class
	extern string classname;
	int pclass();
	
	// >> state
	Tokenizer tok;
	vecs presults;
	Node program({});
	bool dotrace = false;

	// >> error handling
	int error(const string& s) {
		throw parse_error(s + " :: line " + to_string(tok.linepos()) + ", at \"" + tok.peek() + "\"");
	}
	int error_expected(const string& s) {
		return error("expected: " + s);
	}
	int error_unexpected() {
		return error("unexpected token (" + tok.peek() + ")");
	}
	void trace(const string& s) {
		if (dotrace)
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
			else if (rule == "$type" && istype(tok.peek()))
				results.push_back(tok.get());
			else if (rule == "$number" && isnumber(tok.peek()))
				results.push_back(tok.get());
			// else if (rule == "$literal" && )
			else if (rule == "$EOF" && tok.eof())
				results.push_back(tok.get());
			else if (rule == tok.peek())
				results.push_back(tok.get());
			else
				return tok.pos = pos, results = {}, false;
		}
		// all found
		return true;
	}
	int peek(const string& rulesstr, vecs& results = presults) {
		const int pos = tok.pos;
		if (!accept(rulesstr, results))  return tok.pos = pos, false;
		return true;
	}
	int require(const string& rulesstr, vecs& results = presults) {
		if (!accept(rulesstr, results))
			error("missing rule: \"" + rulesstr + "\"");
		return true;
	}
	int istype(const string& str) {
		for (auto& t : basetypes)
			if (t == str)  return true;
		return false;
	}

	// >> begin parsing
	void reset() {
		tok.reset();
		presults = {};
		program = {
			{ "info",
				{ "author", "Professor of Virtual Anthropology" },
				{ "mainclass" }
			}
		};
	}
	int pfile(const string& fname) {
		reset();
		try {
			if (!tok.tokenize(fname))
				error(tok.errormsg);
			if (dotrace)  tok.show();
			pclass();
			trace(program.tostr());
			return true;
		}
		catch (parse_error& p) {
			trace(program.tostr());
			throw p;
		}
	}
}