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
	static EdgeSet symDiff(const EdgeSet &a, const EdgeSet &b);
	static void printEdgeSet(const EdgeSet &es, const string &title);

  public:
	void compute(const vector<Edge> &mst, const Graph<int> &g);
	void printBasis() const;
	void interactiveSymDiff() const;
};
