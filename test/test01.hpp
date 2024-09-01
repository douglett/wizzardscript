#pragma once
#include "unittest.hpp"

struct Test01 : public UnitTest {
	void dotests() {
		WizParse::pfile("test/test01.wizz");
		WizRun::run(WizParse::program);
		expect( "print output", ss.str() == "1 2 3\n" );
	}
};