#pragma once
#include <string>
#include <vector>
#include <cassert>
#include "tokenizer.hpp"
// #include "helpers.hpp"

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
		// "int", "float", "bool", "string"
	};
	struct Node {
		enum Type { T_NIL=0, T_NUMBER, T_STRING, T_LIST };
		Type type = T_LIST;
		int num = 0;
		string str;
		vector<Node> list;
		Node() {}
		Node(int n) { type = T_NUMBER; num = n; }
		// Node(const string& s) { type = T_STRING; str = s; }
		Node(const char* s) { type = T_STRING; str = s; }
		Node(const vector<Node>& vn) { type = T_LIST; list = vn; }
		Node(initializer_list<Node> init) { type = T_LIST; list = {}; for (const auto& n : init) list.push_back(n); }
		Node& push(const Node& n) { assert(type == T_LIST); list.push_back(n); return list.back(); }
	};

	// >> forward-define
	//   .class
	extern string classname;
	int pclass();
	
	// >> state
	Tokenizer tok;
	Node program;
	vecs presults;

	// >> error handling
	int error(const string& s) {
		throw parse_error(s + " :: line " + to_string(tok.linepos()) + ", at \"" + tok.peek() + "\"");
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
			// else if (rule == "$type" && )
			// else if (rule == "$number" && )
			// else if (rule == "$literal" && )
			else if (rule == "$EOF" && tok.eof())
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

	// >> begin parsing
	int pfile(const string& fname) {
		if (!tok.tokenize(fname))
			error(tok.errormsg);
		return pclass();
	}
}