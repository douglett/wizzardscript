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

	// access
	Node& operator[](size_t pos) {
		assert(type == T_LIST);
		return list.at(pos);
	}
	Node& push(const Node& n) {
		assert(type == T_LIST);
		list.push_back(n);
		return list.back();
	}
	Node& findExpression(const string& name) {
		assert(type == T_LIST);
		for (auto& n : list)
			if (n.type == T_LIST && n.list.size() && n[0].type == T_STRING && n[0].str == name)
				return n;
		throw out_of_range("node not found: " + name);
	}
};