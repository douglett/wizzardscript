#pragma once
#include "unittest.hpp"

struct Test01 : public UnitTest {
	void dotests() {
		WizParse::pfile("test/01_print.wizz");
		WizRun::run(WizParse::program);
		expect( "print output", ss.str() == "1 2 3\n" );
		
		WizParse::pfile("test/02_class2.wizz");
		resetio();
		WizRun::run(WizParse::program);
		expect( "print output", ss.str() == "1 2 3\n" );
	}
};