#pragma once

#include "common/graph/graph.h"
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class MinCostFlow {
  private:
	int n;
	vector<vector<T>> cap; 
	vector<vector<T>> cost;

	pair<vector<int>, T> findMinCostPath(int s, int t) const;

  public:
	MinCostFlow(const Graph<T> &g, const vector<vector<T>> &costMatrix);

	pair<T, T> compute(int s, int t, T targetFlow);

	void printResult(T flow, T totalCost, T target, int s, int t) const;
};

#include "mincost.cpp"
