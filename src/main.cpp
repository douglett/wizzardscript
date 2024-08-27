#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
using namespace std;

int main() {
	printf("parsing...\n");
	WizParse::pfile("test/test.wizz");
}