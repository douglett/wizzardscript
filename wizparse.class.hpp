#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	int pclass() {
		// vecs results;
		require("static class $identifier ;");
		auto classname = presults[2];
		trace("begin class: " + classname);
		trace("end class: " + classname);
		return 1;
	}
}