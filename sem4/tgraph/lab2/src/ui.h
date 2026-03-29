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

	void showGraph(Graph<double> &graph);
	void showAllPaths(Graph<double> &graph);
	vector<int> showFulkerson(Graph<double> &graph);
	void showWarshall(Graph<double> &graph);
	void showComparison(Graph<double> &graph);

  public:
	void run();
};
