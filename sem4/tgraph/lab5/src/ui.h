#pragma once

#include "common/graph/graph.h"
#include "cycle_basis.h"
#include "euler.h"
#include <vector>
using namespace std;

class Lab5UI {
  private:
	bool isDirected = false;
	vector<Edge> mstEdges;
	CycleBasis cycleBasis;
	bool hasMST = false;

	void showEuler(Graph<int> *g);
	void showCycleBasis(Graph<int> *g);
	void showSymDiff();

  public:
	void onNewGraph(bool directed);
	void processChoice(int choice, Graph<int> *g);
};
