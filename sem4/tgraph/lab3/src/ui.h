#pragma once

#include "../../common/graph/generator.h"
#include "maxflow.h"
#include "mincost.h"
#include <utility>
#include <vector>

class Lab3UI {
  private:
	vector<vector<int>> costMatrix;
	int source      = 0;
	int sink        = 0;
	int lastMaxFlow = 0;
	bool hasMaxFlow = false;

	pair<int, int> askSourceSink(int n);
	vector<vector<int>> generateCostMatrix(const Graph<int> &graph);
	void showCost(const vector<vector<int>> &cost, int n);
	int  showMaxFlow(const Graph<int> &graph, int s, int t);
	void showMinCostFlow(const Graph<int> &graph, const vector<vector<int>> &cost,
	                     int s, int t, int targetFlow);

  public:
	void onNewGraph(const Graph<int> &graph, int n);
	void processChoice(int choice, Graph<int> *graph);
};
