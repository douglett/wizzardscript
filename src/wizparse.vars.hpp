#pragma once
#include "wizparse.base.hpp"
#include <vector>

namespace WizParse {
	struct Dim { string type, name; };
	static vector<Dim> scope;

	void scope_reset() {
		scope = {};
	}

	void scope_dim(const string& type, const string& name) {
		for (const auto& dim : scope)
			if (dim.name == name)
				error("redefinition: " + name);
		scope.push_back({ type, name });
	}

	static Dim& scope_find(const string& name) {
		for (auto& dim : scope)
			if (dim.name == name)
				return dim;
		error("variable not defined: " + name);
		throw "error";  // thrown above
	}

	int pvarpath(Node& parent, string& type) {
		if (!accept("$identifier"))  return false;
		auto name = presults[0];
		// verify existance and type
		type = scope_find(name).type;
		// output
		parent.push({ "get_global", classmember(name) });
		return true;
	}
}