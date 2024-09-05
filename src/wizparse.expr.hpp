#pragma once
#include "wizparse.base.hpp"
#include <vector>

namespace WizParse {
	// === Variables ===
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

	Dim& scope_find(const string& name) {
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



	// === Expressions ===
	int pexpr(Node& parent, string& type, bool force) {
		if (pvarpath(parent, type))
			return true;
		else if (accept("$literal"))
			return parent.push( presults[0].c_str() ), type = "string", true;
		else if (accept("$number"))
			return parent.push( stoi(presults[0]) ), type = "int", true;
		else if (force)
			error_expected("expression");
		return type = "", false;
	}

	int pexpras(Node& parent, const string& astype, bool force) {
		string type;
		if (pexpr(parent, type, force)) {
			if (type == astype)
				return true;
			else if (force)
				error_expected("expression type: " + astype);
		}
		return false;
	}
}