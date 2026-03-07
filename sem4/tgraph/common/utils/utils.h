#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
void printMatrix(const vector<vector<T>> &matrix, const string &title);

template <typename T>
void printVector(const vector<T> &vec, const string &title);

template <typename T>
const T INF = numeric_limits<T>::max();

#include "utils.cpp"