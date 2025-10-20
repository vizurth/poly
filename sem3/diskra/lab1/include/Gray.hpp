#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

vector<string> generateGrayCode(int n);
vector<string> fillUniverse(const vector<string>& grayCodes, int m);
map<string, int> vectorToMap(const vector<string>& grayCodes, int m);

