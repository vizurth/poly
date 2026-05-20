#pragma once

#include "lab4/src/kruskal.h"
#include <set>
#include <vector>
using namespace std;

using EdgeSet = set<pair<int, int>>;

class CycleBasis {
  private:
	vector<EdgeSet> basis;
	int n = 0;

	vector<int> pathInMST(int src, int dst, const vector<Edge> &mst) const;

  public:
	void compute(const vector<Edge> &mst, const Graph<int> &g);
	void printBasis() const;
	const vector<EdgeSet> &getBasis() const;
	static EdgeSet symDiff(const EdgeSet &a, const EdgeSet &b);
};
