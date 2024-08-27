#include <iostream>
#include "wizparse.base.hpp"
using namespace std;

int main() {
	printf("tokenizing...\n");
	WizParse::tok.tokenize("test.wizz");
	printf("parsing...\n");
	WizParse::pclass();
}