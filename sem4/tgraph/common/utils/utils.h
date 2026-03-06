#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
void printMatrix(const vector<vector<T>> &matrix, const string &title);

#include "utils.cpp"