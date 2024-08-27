#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "helpers.hpp"
using namespace std;

struct Tokenizer {
	struct Tok { string str; int vpos; int hpos; };
	const Tok TOK_EOF = { "$EOF", -1, -1 };
	vector<Tok> tok;
	string errormsg;
	int pos = 0;

	int tokenize(const string& fname) {
		fstream fs(fname, ios::in);
		if (!fs.is_open())
			return error("error: opening file: " + fname);
		// setup
		string line, t;
		int linepos = 0, hpos = 0;
		#define addtok() ( t.length() ? tok.push_back({ t, linepos, hpos }), t = "", 1 : 0 )
		// parse line-by-line
		while (getline(fs, line)) {
			linepos++;
			hpos = 0;
			for (size_t i = 0; i < line.length(); i++) {
				char c = line[i];
				hpos++;
				if (isspace(c))  addtok();  // spaces
				// line comments
				// string
				else if (c == '"') {
					addtok(), t = c;
					for (i++; i < line.length() && line[i] != '"'; i++)
						t += line[i];
					t += '"';
					if (i >= line.length())
						return error("error: unterminated string, line " + to_string(linepos));
				}
				else if (!isalphanum(c))  addtok(), t += c, addtok();  // special characters
				else  t += c;  // normal characters
			}
		}
		// ok
		show();
		return 1;
	}

	int error(const string& msg) {
		// fprintf(stderr, "%s\n", msg.c_str());
		errormsg = msg;
		return 0;
	}

	void show() {
		cout << "tokens: ";
		for (const auto& t : tok)
			cout << t.str << " ";
		cout << endl;
	}

	int eof() {
		return pos < 0 || pos >= (int)tok.size();
	}
	const string& peek() {
		if (eof())  return TOK_EOF.str;
		return tok[ pos ].str;
	}
	const string& get() {
		if (eof())  return TOK_EOF.str;
		return tok[ pos++ ].str;
	}
};