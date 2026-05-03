#pragma once

#include "../../common/graph/generator.h"
#include "euler.h"

class Lab5UI {
  private:
	bool isDirected = false;

	void showEulerCycle(Graph<int> *graph);

  public:
	void onNewGraph(bool directed);
	void processChoice(int choice, Graph<int> *graph);
};
