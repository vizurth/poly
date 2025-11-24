#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

const int N = 8;
const map<string, string> plusOne = {
		{"a", "b"},
		{"b", "c"},
		{"c", "e"},
		{"d", "g"},
		{"e", "d"},
		{"f", "h"},
		{"g", "f"},
		{"h", "a"},
};
const vector<string> alph = {"a", "b", "c", "d", "e", "f", "g", "h"};