#pragma once
#include <string>
#include <vector>
using namespace std;

struct Node {
	enum Type { T_NUMBER, T_STRING, T_LIST };
	Type type = T_LIST;
	int num = 0;
	string str;
	vector<Node> list;

	// constructors
	// Node() {}
	Node(int n) {
		type = T_NUMBER; num = n;
	}
	// Node(const string& s) { type = T_STRING; str = s; }
	Node(const char* s) { type = T_STRING, str = s; }
	Node(const vector<Node>& vn) { type = T_LIST, list = vn; }
	Node(initializer_list<Node> init) {
		type = T_LIST, list = {};
		for (const auto& n : init)
			list.push_back(n);
	}

	// list methods
	void aslist() const {
		if (type != T_LIST)
			throw runtime_error("Node: expected list");
	}
	int issx(const string& sxname = "") const {
		if (type == T_LIST && list.size() && list[0].type == T_STRING)
			return sxname == "" ? true : list[0].str == sxname;
		return false;
	}
	string sxtype() const {
		return issx() ? list[0].str : "$NULL";
	}
	// const accessors
	const Node& operator[](size_t pos) const {
		aslist();
		return list.at(pos);
	}
	const Node& findsx(const string& name) const {
		aslist();
		for (auto& n : list)
			if (n.issx(name))  return n;
		throw out_of_range("node not found: " + name);
	}
	const string tostr() const {
		switch (type) {
			case Node::T_NUMBER:  return to_string(num);
			case Node::T_STRING:  return str;
			case Node::T_LIST:
				string s = "( ";
				for (const auto& nn : list)
					s += nn.tostr() + " ";
				s += " ) ";
				return s;
		}
		return "$$UNKNOWN$$";  // shouldn't happen
	}
	// mutators
	Node& operator[](size_t pos) {
		aslist();
		return list.at(pos);
	}
	Node& findsx(const string& name) {
		aslist();
		for (auto& n : list)
			if (n.issx(name))  return n;
		throw out_of_range("node not found: " + name);
	}
	Node& push(const Node& n) {
		aslist();
		list.push_back(n);
		return list.back();
	}
	Node pop() {
		aslist();
		auto n = list.back();
		list.pop_back();
		return n;
	}
};