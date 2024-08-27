#pragma once
#include <string>

int isalphanum(char c) {
	return isalnum(c) || c == '_';
}
int isnumber(const std::string& s) {
	for (auto c : s)
		if (!isdigit(c))  return 0;
	return 1;
}
int isidentifier(const std::string& s) {
	if (s.length() == 0)  return 0;
	if (!isalpha(s[0]) && s[0] != '_')  return 0;
	for (size_t i = 1; i < s.length(); i++)
		if ( !isalphanum(s[0]) )  return 0;
	return 1;
}
int isliteral(const std::string& s) {
	return s.length() >= 2 && s.front() == '"' && s.back() == '"';
}