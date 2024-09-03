#pragma once
#include "../src/wizparse.class.hpp"

struct UnitTest {
	const string
		CLEARCOL = "\033[0m",
		RED = "\033[31m",
		GREEN = "\033[32m",
		CYAN = "\033[36m";
	stringstream ss;
	string suitename = "UnitTest", fpath = "test/";
	int testno = 0;

	virtual void setup() {
		WizParse::reset();
		WizRun::reset();
		WizRun::output = &ss;
		ss.str(""), ss.clear();
		testno = 0;
	}
	virtual void teardown() {
		WizRun::output = &cout;
	}
	virtual void dotests() {}

	int runfile(const string& file) {
		WizParse::pfile(fpath + file);
		ss.str(""), ss.clear();
		return WizRun::run(WizParse::program);
	}

	int expect(const string& name, bool value) {
		cout << "> running test: " << name << "... ";
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
			result = 1;
		} catch (runtime_error& e) {
			cerr << RED << ":: ERROR ::" << CLEARCOL << endl
				<< "   " << e.what() << endl;
			if (testno > 0)
				cerr << "   " << "at test: " << testno << endl; 
		}
		teardown();
		return result;
	}
};