#pragma once

#include "common/graph/graph.h"
#include <string>
#include <vector>

using namespace std;

struct EulerChange {
	int    u;
	int    v;
	bool   added;
	string reason;
};

struct EulerResult {
	bool initiallyEulerian = false;
	bool finalEulerian     = false;
	vector<EulerChange> changes;
	vector<int> cycle;
};

class EulerCycle {
  private:
	Graph<int> graph;
	vector<vector<int>> initialAdj;

	int degree(int v) const;
	vector<int> oddVertices() const;
	vector<vector<int>> connectedComponents() const;
	bool isEulerian() const;

	bool hasEdge(int u, int v) const;
	void addEdge(int u, int v, int w);
	bool removeEdge(int u, int v);
	bool isInitialEdge(int u, int v) const;

	void connectComponents(vector<EulerChange> &changes);
	void makeEvenDegrees(vector<EulerChange> &changes);
	vector<int> buildEulerCycle() const;

  public:
	explicit EulerCycle(const Graph<int> &g);

	EulerResult process();
	Graph<int>  getGraph() const;

	static void printReport(const EulerResult &result);
};
