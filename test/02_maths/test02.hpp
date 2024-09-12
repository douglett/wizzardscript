#pragma once
#include "../unittest.hpp"

struct Test02 : public UnitTest {
	Test02() {
		suitename = "Test 02 - Maths";
		fpath += "02_maths/";
	}

	void dotests() {
		testno++;
		runfile("01_int_comparisons.wizz");
		expect( "int comparisons", ssout.str() == "8 2\n" );

		testno++;
		runfile("02_add_mul.wizz");
		expect( "int add and multiply", ssout.str() == "5 27 29\n" );
	}
};