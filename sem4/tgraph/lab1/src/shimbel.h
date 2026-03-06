#pragma once

#include "../../common/graph/graph.h"
#include "../../common/utils/utils.h"

template <typename T>
class ShimbelSolver {
  private:
	const Graph<T> &graph;
	int n;
	vector<vector<T>> currentMatrix;

	static T getINF() { return numeric_limits<T>::max() / 2; }

	void initializeMatrix();
	void multiplyMatrix(bool findMin);
	vector<vector<T>> compute(int k, bool findMin);

  public:
	ShimbelSolver(const Graph<T> &g);

	vector<vector<T>> computeShortestPaths(int k);
	vector<vector<T>> computeLongestPaths(int k);

	static void printShimbelMatrix(const vector<vector<T>> &matrix,
	                               const string &title);
};

#include "shimbel.cpp"