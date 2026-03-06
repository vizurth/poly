#pragma once

#include "../../common/graph/generator.h"
#include "shimbel.h"
#include <string>

class UI {
  private:
	int askNumVertices();
	bool askDirected();
	WeightType askWeightType();

	void showGraph(Graph<double> &graph);
	void showEccentricities(Graph<double> &graph);
	void showCenter(Graph<double> &graph);
	void showDiameter(Graph<double> &graph);
	void showShimbell(Graph<double> &graph);

  public:
	void run();
};
