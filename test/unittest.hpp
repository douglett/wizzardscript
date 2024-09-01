#pragma once
#include "../src/wizparse.class.hpp"

struct UnitTest {
	const string
		CLEARCOL = "\033[0m",
		RED = "\033[31m",
		GREEN = "\033[32m",
		CYAN = "\033[36m";
	stringstream ss;

	virtual string suitename() { return "UnitTest"; }
	virtual void setup() {
		WizParse::reset();
		WizRun::reset();
		WizRun::output = &ss;
		resetio();
	}
	virtual void teardown() {}
	virtual void dotests() {}
	void resetio() { ss.str(""), ss.clear(); }

	int expect(const string& name, bool value) {
		cout << "> running test: " << name << "... ";
		if (value)  cout << GREEN << "OK";
		else        cout << RED << "Fail";
		cout << CLEARCOL << endl;
		return value;
	}

	int runall() {
		cout << CYAN << "Running test suite: " << suitename() << CLEARCOL << endl;
		setup();
		try {
			dotests();
			return 1;
		} catch (runtime_error& e) {
			cerr << e.what() << endl;
			return 0;
		}
		teardown();
	}
};