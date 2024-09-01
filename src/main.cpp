#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
#include "wizparse.vars.hpp"
#include "wizrun.hpp"
#include "../test/test01.hpp"
using namespace std;

void test() {
	printf("parsing...\n");
	WizParse::pfile("test/test.wizz");
	// WizParse::pfile("test/03_int_variables.wizz");
	WizRun::run(WizParse::program);
}

int main() {
	// test();
	Test01().runall();  // run all tests
}