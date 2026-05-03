#pragma once

#include "common/graph/graph.h"
#include <vector>
using namespace std;

/*
    Максимальное независимое множество вершин (MIS).
    Строго по псевдокоду со слайдов (Поиск с возвратами / Backtracking).
*/
class MIS {
  private:
	int n;
	vector<vector<int>> adj;

	vector<int> X; // итоговое максимальное независимое множество
	int m;         // размер текущего лучшего решения

	bool isIndependent(const vector<int> &S, int v) const;

	void backtrack(vector<int> S, vector<int> T);

  public:
	MIS(const vector<vector<int>> &adjMatrix);

	vector<int> compute();

	static void printResult(const vector<int> &mis);
};