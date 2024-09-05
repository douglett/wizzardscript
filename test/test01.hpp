#pragma once
#include "unittest.hpp"

struct Test01 : public UnitTest {
	Test01() {
		suitename = "Test01";
	}

	void dotests() {
		testno++;
		runfile("01_print.wizz");
		expect( "testing print", ssout.str() == "1 2 3\n" );
		
		testno++;
		runfile("02_class2.wizz");
		expect( "testing bracketed classes", ssout.str() == "1 2 3\n" );

		testno++;
		runfile("03_int_variables.wizz");
		expect( "testing int variables", ssout.str() == "4\n23\n" );

		testno++;
		runfile("04_strings.wizz");
		expect( "testing strings", ssout.str() == "1: blah\n2: hello\n" );

		testno++;
		ssin.str("1\n"), ssin.clear();
		runfile("05_stdio.wizz");
		expect( "testing input", ssout.str() == "type 1: you wrote 1\n" );
	}
};