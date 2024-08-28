#pragma once
#include "node.hpp"

namespace WizRun {
	using namespace std;

	Node program({});

	int run(const Node& prog) {
		program = prog;

		// run static-init
		// run main
	}
}