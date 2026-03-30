#pragma once

#include "../../common/graph/generator.h"
#include "fulkerson.h"
#include "warshall.h"
#include <string>

class UI {
  private:
	int askNumVertices();
	bool askDirected();
	WeightType askWeightType();

	void showGraph(Graph<int> &graph);
	void showAllPaths(Graph<int> &graph);
	vector<int> showFulkerson(Graph<int> &graph);
	void showWarshall(Graph<int> &graph);
	void showComparison(Graph<int> &graph);

  public:
	void run();
};
