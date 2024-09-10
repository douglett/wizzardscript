#pragma once
#include "../unittest.hpp"

struct Test01 : public UnitTest {
	Test01() {
		suitename = "Test01";
		fpath += "01_basic/";
	}

	void dotests() {
		testno++;
		runfile("01_print.wizz");
		expect( "print", ssout.str() == "1 2 3\n" );
		
		testno++;
		runfile("02_class2.wizz");
		expect( "bracketed classes", ssout.str() == "1 2 3\n" );

		testno++;
		runfile("03_int_variables.wizz");
		expect( "int variables", ssout.str() == "4\n23\n" );

		testno++;
		runfile("04_strings.wizz");
		expect( "strings", ssout.str() == "1: blah\n2: hello\n" );

		testno++;
		ssin.str("1\n"), ssin.clear();
		runfile("05_stdio.wizz");
		expect( "input", ssout.str() == "type 1: you wrote 1\n" );

		testno++;
		runfile("06_if_else.wizz");
		WizRun::mem["test__a"] = 2,  WizRun::rcall("test__main");
		WizRun::mem["test__a"] = 3,  WizRun::rcall("test__main");
		expect( "if-else", ssout.str() == "hello\nwelcome\ngoodbye\n" );

		testno++;
		runfile("07_while.wizz");
		expect( "while", ssout.str() == "1\n2\n3\n4\n5\n" );

		testno++;
		runfile("08_functions.wizz");
		expect( "functions", ssout.str() == "hello 3\n" );
	}
};