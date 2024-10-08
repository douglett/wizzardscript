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

		testno++;
		runfile("03_str_as_array.wizz");
		expect( "string as array", ssout.str() == "hello\n104\nhallo\n" );

		testno++;
		runfile("04_array_of_str.wizz");
		expect( "array of strings", ssout.str() == "hello  world\n" );

		testno++;
		runfile("05_length.wizz");
		expect( "array length", ssout.str() == "1 6\nhello 5\n" );
	}
};