#pragma once
#include "unittest.hpp"

struct Test01 : public UnitTest {
	Test01() {
		suitename = "Test01";
	}

	void dotests() {
		runfile("01_print.wizz");
		expect( "testing print", ss.str() == "1 2 3\n" );
		
		runfile("02_class2.wizz");
		expect( "testing bracketed classes", ss.str() == "1 2 3\n" );

		runfile("03_int_variables.wizz");
		expect( "testing int variables", ss.str() == "4\n23\n" );
	}
};