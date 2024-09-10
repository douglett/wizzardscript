#pragma once
#include "../unittest.hpp"

struct Test02 : public UnitTest {
	Test02() {
		suitename = "Test02";
		fpath += "02_maths/";
	}

	void dotests() {
		testno++;
		runfile("01_int_comparisons.wizz");
		expect( "int comparisons", ssout.str() == "8 2\n" );
	}
};