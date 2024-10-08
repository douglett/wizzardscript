#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
#include "wizparse.expr.hpp"
#include "wizrun.hpp"
#include "../test/01_basic/test01.hpp"
#include "../test/02_maths/test02.hpp"
#include "../test/03_arrays/test03.hpp"
using namespace std;

void test() {
	printf("\nparsing test.wizz...\n");
	WizParse::dotrace = true;
	WizParse::pfile("test/test.wizz");
	// WizParse::pfile("test/03_arrays/04_array_of_str.wizz");
	// stringstream ss("1\n");
	// WizRun::input = &ss;
	printf("\nrunning...\n");
	WizRun::run(WizParse::program);
}

int main() {
	// run all tests
	Test01().runall();
	Test02().runall();
	Test03().runall();
	// WIP script
	test();
}