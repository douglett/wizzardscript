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
	void aslist() {
		if (type != T_LIST)
			throw runtime_error("Node: expected list");
	}
	Node& operator[](size_t pos) {
		aslist();
		return list.at(pos);
	}
	Node& push(const Node& n) {
		aslist();
		list.push_back(n);
		return list.back();
	}
	int issx(const string& sxname = "") {
		if (type == T_LIST && list.size() && list[0].type == T_STRING)
			return sxname == "" ? true : list[0].str == sxname;
		return false;
	}
	Node& findsx(const string& name) {
		aslist();
		for (auto& n : list)
			if (n.issx(name))  return n;
		throw out_of_range("node not found: " + name);
	}
};