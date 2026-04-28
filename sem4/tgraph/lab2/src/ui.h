#pragma once

#include "../../common/graph/generator.h"
#include "fulkerson.h"
#include "warshall.h"

class Lab2UI {
  private:
	vector<int> showFulkerson(Graph<int> &graph);
	void showWarshall(Graph<int> &graph);
	void showComparison(Graph<int> &graph);

  public:
	void processChoice(int choice, Graph<int> *graph);
};
