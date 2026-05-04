#pragma once

#include "common/graph/graph.h"
#include "lab4/src/kruskal.h"
#include <utility>
#include <vector>

using namespace std;

struct SymDiffResult {
	vector<pair<int, int>> edges;
	vector<vector<int>>    contours;
};

class CycleBasis {
  private:
	Graph<int> graph;
	vector<vector<int>> mstAdj;
	vector<Edge> mstEdges;
	vector<vector<int>> fundamentalCycles;

	static pair<int, int> normalizeEdge(int u, int v);
	vector<int> buildEulerCycle(vector<vector<int>> &adj, int start) const;

  public:
	explicit CycleBasis(const Graph<int> &g);

	bool build();

	const vector<Edge> &getMSTEdges() const;
	const vector<vector<int>> &getFundamentalCycles() const;

	SymDiffResult symmetricDifference(const vector<int> &indices1Based) const;

	static void printFundamentalCycles(const vector<vector<int>> &cycles);
	static void printSymDiffResult(const SymDiffResult &result);
};
