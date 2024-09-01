#pragma once
#include "../src/wizparse.class.hpp"

struct UnitTest {
	stringstream ss;

	virtual void setup() {
		WizParse::reset();
		WizRun::reset();
		WizRun::output = &ss;
		ss.str(""), ss.clear();
	}
	virtual void teardown() {}
	virtual void dotests() {}

	int expect(const string& name, bool value) {
		cout << "> running test: " << name << "... ";
		if (value)  cout << "\033[32m" << "OK";
		else        cout << "\033[31m" << "Fail";
		cout << "\033[0m" << endl;
		return value;
	}

	int runall() {
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