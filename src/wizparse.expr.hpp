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
	static int pex_or(Node& parent, string& lhs);
	static int pex_equals(Node& parent, string& lhs);
	static int pex_greater(Node& parent, string& lhs);
	static int pex_add(Node& parent, string& lhs);
	static int pex_atom(Node& parent, string& type);
	int pex_test(Node& parent, string exprtype, string& lhs, string& rhs);

	int pexpr(Node& parent, string& type, bool force) {
		if (pex_or(parent, type))
			return true;
		else if (force)
			error_expected("expression");
		return type = "", false;
	}

	int pexpras(Node& parent, const string& astype, bool force) {
		int pos = tok.pos;
		string type;
		if (pex_or(parent, type)) {
			if (type == astype)
				return true;
			else if (force)
				error_expected("expression type: " + astype);
			else
				tok.pos = pos, parent.pop();
		}
		return false;
	}

	// TODO: implement
	static int pex_or(Node& parent, string& lhs) {
		// return pex_equals(parent, lhs);
		string rhs;
		return pex_test(parent, "equals", lhs, rhs);
	}

	static int pex_equals(Node& parent, string& lhs) {
		if (!pex_greater(parent, lhs))  return false;
		if (accept("= =") || accept("! =")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_greater(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_greater(Node& parent, string& lhs) {
		if (!pex_add(parent, lhs))  return false;
		if (accept("> =") || accept(">") || accept("< =") || accept("<")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_add(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_add(Node& parent, string& lhs) {
		if (!pex_atom(parent, lhs))  return false;
		if (accept("+") || accept("-")) {
			string rhs, op = presults[0];
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_atom(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_atom(Node& parent, string& type) {
		if (accept("true") || accept("false"))
			return parent.push( presults[0].c_str() ), type = "int", true;
		else if (pvarpath(parent, type))
			return true;
		else if (accept("$literal"))
			return parent.push( presults[0].c_str() ), type = "string", true;
		else if (accept("$number"))
			return parent.push( stoi(presults[0]) ), type = "int", true;
		return type = "", false;
	}



	// >> expression test functions
	
	// int pex_nextfn_(Node& parent, string& type, const string& exprtype) {
	// 	if      (exprtype == "equals")   return pex_greater(parent, type);
	// 	else if (exprtype == "greater")  return pex_add(parent, type);
	// 	else if (exprtype == "add")      return pex_atom(parent, type);
	// 	return error(exprtype);
	// }

	int pex_nextfn(Node& parent, string& type, const string& exprtype) {
		string rhs;
		if      (exprtype == "equals")   return pex_test(parent, "greater", type, rhs);
		else if (exprtype == "greater")  return pex_test(parent, "add", type, rhs);
		else if (exprtype == "add")      return pex_atom(parent, type);
		return error("nextfn: " + exprtype);
	}

	int pex_opmatch(const string& exprtype) {
		if (exprtype == "equals")   return accept("= =") || accept("! =");
		if (exprtype == "add")      return accept("+") || accept("-");
		if (exprtype == "greater")  return accept("> =") || accept(">") || accept("< =") || accept("<");
		return error("opmatch: " + exprtype);
	}

	int pex_test(Node& parent, string exprtype, string& lhs, string& rhs) {
		if (!pex_nextfn(parent, lhs, exprtype))  return false;
		if (pex_opmatch(exprtype)) {
			string op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_nextfn(stmt, rhs, exprtype) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}
}