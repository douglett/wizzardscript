#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"
// #include "helpers.hpp"
using namespace std;

namespace WizParse {
	Tokenizer tok;
	const vector<string> keywords = {
		"static", "class",
		"print"
		// "if", "else", "while", "return",
		// "push", "pop", "concat", "clear",
		// "int", "float", "bool", "string"
	};
	
	int pclass() {
		require("static class $identifier ;");
		return 0;
	}
};