#pragma once

#include "common/graph/graph.h"
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class MaxFlow {
  private:
	int n;
	vector<vector<T>> cap;

	vector<int> bfs(int s, int t) const; //  кратчайщий путь

  public:
	MaxFlow(const Graph<T> &g);

	T compute(int s, int t); // O(VE^2)

	void printResult(T flow, int s, int t) const;
};

#include "maxflow.cpp"
