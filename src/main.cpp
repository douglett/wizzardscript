#include <iostream>
#include "wizparse.base.hpp"
#include "wizparse.class.hpp"
#include "wizrun.hpp"
using namespace std;

int main() {
	printf("parsing...\n");
	WizParse::pfile("test/test.wizz");
	WizRun::run(WizParse::program);
}