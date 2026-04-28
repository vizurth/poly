#pragma once

#include "common/graph/graph.h"
#include <vector>
using namespace std;

/*
    Максимальное независимое множество вершин (MIS).
    Алгоритм: поиск с возвратами (Backtracking).

    BT(S, T):
      if T = ∅: обновить наилучший результат
      for v ∈ T:
        if IS_INDEPENDENT(S, v): BT(S ∪ {v}, T \ Γ*(v))
        T := T \ {v}
*/
class MIS {
  private:
	int n;
	vector<vector<int>> adj;
	vector<int> bestSet;

	bool isIndependent(const vector<int> &S, int v) const;
	void backtrack(vector<int> S, vector<int> T);

  public:
	explicit MIS(const Graph<int> &g);

	vector<int> compute();

	static void printResult(const vector<int> &mis);
};
