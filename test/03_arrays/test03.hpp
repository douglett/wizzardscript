#pragma once
#include "../unittest.hpp"

struct Test03 : public UnitTest {
	Test03() {
		suitename = "Test 03 - Arrays";
		fpath += "03_arrays/";
	}

	void dotests() {
		testno++;
		runfile("01_array_dim.wizz");
		expect( "array dim", ssout.str() == "OK\n" );

		testno++;
		runfile("02_get_assign.wizz");
		expect( "array get + assign", ssout.str() == "0 101 0\n" );
	}
};