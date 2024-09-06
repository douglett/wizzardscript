#pragma once
#include "../src/wizparse.class.hpp"

struct UnitTest {
	const string
		CLEARCOL = "\033[0m",
		RED = "\033[31m",
		GREEN = "\033[32m",
		CYAN = "\033[36m";
	stringstream ssout, ssin;
	string suitename = "UnitTest", fpath = "test/";
	int testno = 0;

	virtual void setup() {
		WizParse::reset();
		WizRun::reset();
		WizRun::output = &ssout;
		WizRun::input = &ssin;
		ssout.str(""), ssout.clear();
		ssin.str(""), ssin.clear();
		testno = 0;
	}
	virtual void teardown() {
		WizRun::output = &cout;
		WizRun::input = &cin;
	}
	virtual void dotests() {}

	int runfile(const string& file) {
		WizParse::pfile(fpath + file);
		ssout.str(""), ssout.clear();
		// ssin.str(""), ssin.clear();
		return WizRun::run(WizParse::program);
	}

	int expect(const string& name, bool value) {
		cout << "> running test " << (testno < 10 ? "0" : "") << testno << ": " << name << "... ";
		if (value)  cout << GREEN << "OK";
		else        cout << RED << "Fail";
		cout << CLEARCOL << endl;
		return value;
	}

	int runall() {
		cout << CYAN << "Running test suite: " << suitename << CLEARCOL << endl;
		int result = 0;
		setup();
		try {
			dotests();
			cout << GREEN <<  "All tests passed" << CLEARCOL << endl;
			result = 1;
		} catch (runtime_error& e) {
			cout << RED << "ERROR in test " << testno << CLEARCOL << endl;
			cout << "   " << e.what() << endl;
		}
		teardown();
		return result;
	}
};