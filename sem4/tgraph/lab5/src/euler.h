#pragma once

#include "common/graph/graph.h"
#include <optional>
#include <vector>
using namespace std;

class EulerianCycle {
  private:
	Graph<int> g;
	int n;
	vector<pair<int, int>> addedEdges;

	int degree(int v) const;
	vector<int> oddDegreeVertices() const;
	vector<vector<int>> nonZeroComponents() const;
	optional<vector<int>> hierholzer(int start) const;

  public:
	explicit EulerianCycle(const Graph<int> &graph);

	bool isEulerian() const;
	bool isSemiEulerian() const;
	void makeEulerian();
	optional<vector<int>> findCycle();

	const vector<pair<int, int>> &getAddedEdges() const;

	static void printCycle(const vector<int> &cycle);
	static void printAddedEdges(const vector<pair<int, int>> &added);
};
