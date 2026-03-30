#pragma once

#include "common/graph/graph.h"
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Fulkerson {
  private:
	const Graph<T> &graph;
	int n;
	mutable int iterCount = 0;

	vector<int> findRoots(const vector<bool> &removed) const;

  public:
	Fulkerson(const Graph<T> &g);

	vector<int> computeOrder() const;
	int getIterCount() const { return iterCount; }

	void printResult(const vector<int> &order) const;
};

#include "fulkerson.cpp"