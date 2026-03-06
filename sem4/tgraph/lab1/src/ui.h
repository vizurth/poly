#pragma once

#include "../../common/graph/generator.h"
#include <string>

class UI {
  private:
	int askNumVertices();
	bool askDirected();
	WeightType askWeightType();

	void showGraph(Graph<double> &graph);

  public:
	void run();
};
