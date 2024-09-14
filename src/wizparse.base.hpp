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
	const vector<string> basetypes = { "int", "string" };

	// >> forward-define
	int istype(const string& str);
	// .class
	const char* classmember(const string& name);
	int pclass();
	// .epxr
	struct Dim { string type, name; };
	struct FnDef { string type, name; vector<Dim> args; };
	void scope_reset();
	void scope_dim(const string& type, const string& name);
	Dim& scope_find(const string& name);
	void func_def(const string& type, const string& name, const vector<Dim>& args);
	FnDef& func_find(const string& name);
	int pvarpath(Node& parent, string& type);
	int pexpr(Node& parent, string& type, bool force = true);
	int pexpras(Node& parent, const string& astype, bool force = true);
	
	// >> state
	Tokenizer tok;
	vecs presults;
	Node program({});
	bool dotrace = false;

	// >> error handling
	int error(const string& s) {
		throw parse_error(s + " :: line " + to_string(tok.linepos())); // + ", at \"" + tok.peek() + "\"");
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
			bool found = false;
			if      (rule == "$identifier")  found = isidentifier(tok.peek());
			else if (rule == "$type")        found = istype(tok.peek());
			else if (rule == "$number")      found = isnumber(tok.peek());
			else if (rule == "$literal")     found = isliteral(tok.peek());
			else if (rule == "$EOF")         found = tok.eof();
			else if (rule[0] == '$')         error("unknown rule: " + rule);
			else if (rule == tok.peek())     found = true;
			// accept rule
			if (found)
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
		scope_reset();
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