#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

// const int N = 8;
// const map<string, string> plusOne = {
// 		{"0", "1"},
// 		{"1", "2"},
// 		{"2", "3"},
// 		{"3", "4"},
// 		{"4", "5"},
// 		{"5", "6"},
// 		{"6", "7"},
// 		{"7", "0"},
// };
// const vector<string> alph = {"0", "1", "2", "3", "4", "5", "6", "7"};

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