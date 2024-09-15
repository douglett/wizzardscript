#pragma once
#include "wizparse.base.hpp"
#include <vector>

namespace WizParse {
	// === Variables ===
	static vector<Dim> gscope, scope;
	static vector<FnDef> fndef;

	void scope_reset() {
		gscope = {};
		scope = {};
		fndef = {};
	}


	// === Variables ===
	void scope_gdim(const string& type, const string& name) {
		for (const auto& dim : gscope)
			if (dim.name == name)
				error("redefinition: " + name);
		gscope.push_back({ type, name });
	}
	void scope_dim(const string& type, const string& name) {
		for (const auto& dim : scope)
			if (dim.name == name)
				error("redefinition: " + name);
		scope.push_back({ type, name });
	}
	void scope_clear() {
		scope = {};
	}
	int pvarpath(Node& parent, string& type) {
		if (!accept("$identifier"))  return false;
		auto name = presults[0];
		// local variable
		type = "";
		for (auto& dim : scope)
			if (dim.name == name)
				type = dim.type, parent.push({ "get_local", classmember(name) });
		// global variable
		if (type == "")
			for (auto& dim : gscope)
				if (dim.name == name)
					type = dim.type, parent.push({ "get_global", classmember(name) });
		// missing variable
		if (type == "")
			error("variable not defined: " + name);
		// array subset
		if (accept("[")) {
			auto& get = parent.push({ "get_offset", parent.pop() });
			pexpras(get, "int");
			require("]");
			if (isarray(type))
				type = basetype(type);
			else if (type == "string")
				type = "int";
			else
				error("expected array type: " + type);
		}
		// output
		return true;
	}


	// === Function definitions ===
	void func_def(const FnDef& def) {
		for (const auto& d : fndef)
			if (d.name == def.name)
				error("function redefinition: " + def.name);
		fndef.push_back(def);
	}
	FnDef& func_find(const string& name) {
		for (auto& def : fndef)
			if (def.name == name)
				return def;
		error("function not defined: " + name);
		throw "error";  // thrown above
	}


	// === Expressions ===
	static int pex_or(Node& parent, string& lhs);
	static int pex_and(Node& parent, string& lhs);
	static int pex_equals(Node& parent, string& lhs);
	static int pex_greater(Node& parent, string& lhs);
	static int pex_add(Node& parent, string& lhs);
	static int pex_mul(Node& parent, string& lhs);
	static int pex_atom(Node& parent, string& type);

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

	// internal expression functions
	static int pex_or(Node& parent, string& lhs) {
		if (!pex_and(parent, lhs))
			return false;
		while (accept("| |")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_and(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_and(Node& parent, string& lhs) {
		if (!pex_equals(parent, lhs))
			return false;
		while (accept("& &")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_equals(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_equals(Node& parent, string& lhs) {
		if (!pex_greater(parent, lhs))
			return false;
		if (accept("= =") || accept("! =")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_greater(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_greater(Node& parent, string& lhs) {
		if (!pex_add(parent, lhs))
			return false;
		if (accept("> =") || accept(">") || accept("< =") || accept("<")) {
			string rhs, op = joinstr(presults, "");
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_add(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_add(Node& parent, string& lhs) {
		if (!pex_mul(parent, lhs))
			return false;
		while (accept("+") || accept("-")) {
			string rhs, op = presults[0];
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_mul(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	static int pex_mul(Node& parent, string& lhs) {
		if (!pex_atom(parent, lhs))
			return false;
		while (accept("*") || accept("/")) {
			string rhs, op = presults[0];
			auto& stmt = parent.push({ op.c_str(), parent.pop() });
			if (!pex_atom(stmt, rhs) || lhs != rhs || lhs != "int")
				error_expected(op + ": int expression on right-hand");
		}
		return true;
	}

	// sub-atomic
	static int pex_call(Node& parent, string& type) {
		if (!accept("$identifier ("))  return false;
		auto name = presults[0];
		type = func_find(name).type;
		parent.push({ "call", name.c_str(), {} });
		// TODO: function arguments
		require(")");
		return true;
	}

	static int pex_membercall(Node& parent, string& type) {
		int pos = tok.pos;
		if (!pvarpath(parent, type))  return false;
		if (!accept("."))  return tok.pos = pos, parent.pop(), false;
		require("$identifier ( )");
		auto name = presults[0];
		// built in member calls
		if (name == "length" && (isarray(type) || type == "string"))
			parent.push({ "length", parent.pop() }), type = "int";
		else
			error("unexpected member call: " + name + " on " + type);
		return true;
	}

	static int pex_atom(Node& parent, string& type) {
		if (accept("true") || accept("false"))
			return parent.push( presults[0].c_str() ), type = "int", true;
		else if (pex_call(parent, type))
			return true;
		else if (pex_membercall(parent, type))
			return true;
		else if (pvarpath(parent, type))
			return true;
		else if (accept("$literal"))
			return parent.push( presults[0].c_str() ), type = "string", true;
		else if (accept("$number"))
			return parent.push( stoi(presults[0]) ), type = "int", true;
		return type = "", false;
	}
}