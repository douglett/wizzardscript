#pragma once
#include "wizparse.base.hpp"

namespace WizParse {
	int pvarpath(Node& parent, string& type) {
		if (!accept("$identifier"))  return false;
		auto name = presults[0];
		// verify existance and type 
		type = "int";
		// output
		parent.push({ "get_global", classmember(name) });
		return true;
	}
}