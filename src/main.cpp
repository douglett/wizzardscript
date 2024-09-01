#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
#include "wizparse.vars.hpp"
#include "wizrun.hpp"
#include "../test/test01.hpp"
using namespace std;

int main() {
	printf("parsing...\n");
	WizParse::pfile("test/test.wizz");
	WizRun::run(WizParse::program);

	// run all tests
	// Test01().runall();
}