#pragma once
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// parsing
int isalphanum(char c) {
	return isalnum(c) || c == '_';
}
int isnumber(const string& s) {
	for (auto c : s)
		if (!isdigit(c))  return 0;
	return 1;
}
int isidentifier(const string& s) {
	if (s.length() == 0)  return 0;
	if (!isalpha(s[0]) && s[0] != '_')  return 0;
	for (size_t i = 1; i < s.length(); i++)
		if ( !isalphanum(s[0]) )  return 0;
	return 1;
}
int isliteral(const string& s) {
	return s.length() >= 2 && s.front() == '"' && s.back() == '"';
}
int isarray(const string& s) {
	return s.substr(s.length()-2, 2) == "[]" && isidentifier(s.substr(0, s.length()-2));
}
string basetype(const string& type) {
	return isarray(type) ? type.substr(0, type.length()-2) : type;
}

// strings
string stripliteral(const string& str) {
	return isliteral(str) ? str.substr(1, str.length()-2) : str;
}
vector<string> splitstr(const string str) {
	vector<string> vs;
	stringstream ss(str);
	string s;
	while (ss >> s)
		vs.push_back(s);
	return vs;
}
string joinstr(const vector<string>& vs, const string& glue = " ") {
	string str;
	for (auto& s : vs)
		str += (str.length() ? glue : "") + s;
	return str;
}