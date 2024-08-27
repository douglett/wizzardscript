#include <iostream>
#include "wizparse.hpp"
using namespace std;

int main() {
	printf("hello world!\n");
	WizParse::tok.tokenize("test.wizz");
	WizParse::pclass();
}