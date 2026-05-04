#pragma once

#include "../../common/graph/generator.h"
#include "cycle_basis.h"
#include "euler.h"

class Lab5UI {
  private:
	bool isDirected = false;

	void showEulerCycle(Graph<int> *graph);
	void showCycleBasis(Graph<int> *graph);
	vector<int> askCycleIndices(int totalCycles);

  public:
	void onNewGraph(bool directed);
	void processChoice(int choice, Graph<int> *graph);
};
