#pragma once
#include "common/graph/graph.h"
#include <vector>
using namespace std;

class EulerianCycle {
  private:
	Graph<int> g;
	int n;
	int degree(int v) const;
	vector<int> oddVertices() const;
	bool isBridge(int u, int v) const;

  public:
	explicit EulerianCycle(const Graph<int> &graph);
	bool isEulerian() const;
	void makeEulerian();
	vector<int> buildCycle() const;
	const Graph<int> &getGraph() const;
	static void printCycle(const vector<int> &cycle);
};