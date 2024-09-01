#pragma once
#include "unittest.hpp"

struct Test01 : public UnitTest {
	Test01() {
		suitename = "Test01";
	}

	void dotests() {
		runfile("01_print.wizz");
		expect( "print output", ss.str() == "1 2 3\n" );
		
		runfile("02_class2.wizz");
		expect( "print output", ss.str() == "1 2 3\n" );

		runfile("03_int_variables.wizz");
		expect( "print output", ss.str() == "4\n23\n" );
	}
};