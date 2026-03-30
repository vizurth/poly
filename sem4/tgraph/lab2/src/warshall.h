#pragma once

#include "common/graph/graph.h"
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

template <typename T>
class FloydWarshall {
  private:
	const Graph<T> &graph;
	int n;

	vector<vector<T>> distMatrix;  
	vector<vector<int>> nextMatrix;
	int iterCount = 0;             

	static T getPosINF() { return numeric_limits<T>::max() / 2; }
	static T getNegINF() { return -numeric_limits<T>::max() / 2; }

	void initMatrices();

  public:
	FloydWarshall(const Graph<T> &g);

	void compute();

	vector<int> getPath(int from, int to) const;

	const vector<vector<T>> &getDistMatrix() const { return distMatrix; }
	const vector<vector<int>> &getNextMatrix() const { return nextMatrix; }
	int getIterCount() const { return iterCount; }

	void printDistMatrix() const;
	void printPath(int from, int to) const;
};

#include "warshall.cpp"