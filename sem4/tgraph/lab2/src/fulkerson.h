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

	vector<int> findRoots(const vector<bool> &removed) const;

  public:
	Fulkerson(const Graph<T> &g);

	// topologicalOrder[i] = новый номер вершины i
	vector<int> computeOrder() const;

	void printResult(const vector<int> &order) const;
};

#include "fulkerson.cpp"