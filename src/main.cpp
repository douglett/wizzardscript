#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
#include "wizparse.expr.hpp"
#include "wizrun.hpp"
#include "../test/test01.hpp"
using namespace std;

void test() {
	printf("\nparsing test.wizz...\n");
	WizParse::dotrace = true;
	WizParse::pfile("test/test.wizz");
	// WizParse::pfile("test/03_int_variables.wizz");
	WizRun::run(WizParse::program);
}

int main() {
	Test01().runall();  // run all tests
	test();
}