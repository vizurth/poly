#pragma once

#include "../../common/graph/generator.h"
#include "shimbel.h"

class Lab1UI {
  private:
	void showEccentricities(Graph<int> &graph);
	void showCenter(Graph<int> &graph);
	void showDiameter(Graph<int> &graph);
	void showShimbell(Graph<int> &graph);
	void showAllPaths(Graph<int> &graph);

  public:
	void processChoice(int choice, Graph<int> *graph);
};
