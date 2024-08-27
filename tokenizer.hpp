#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "helpers.hpp"
using namespace std;

struct Tokenizer {
	struct Tok { string str; int vpos; int hpos; };
	vector<Tok> tok;
	int pos = 0;

	int tokenize(const string& fname) {
		fstream fs(fname, ios::in);
		if (!fs.is_open())
			return fprintf(stderr, "error opening file: %s\n", fname.c_str()), 0;
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
						return fprintf(stderr, "error, unterminated string line %i\n", linepos), 0;
				}
				else if (!isalphanum(c))  addtok(), t += c, addtok();  // special characters
				else  t += c;  // normal characters
			}
		}
		// ok
		show();
		return 1;
	}

	void show() {
		cout << "tokens: ";
		for (const auto& t : tok)
			cout << t.str << " ";
		cout << endl;
	}

	// int eof() {
	// }
};