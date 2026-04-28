#pragma once

#include "../../common/graph/generator.h"

#include "kirchhoff.h"
#include "kruskal.h"
#include "mis.h"
#include "prufer.h"

class Lab4UI {
  private:
	vector<Edge>  mstEdges;
	PruferCode    pruferCode;
	bool          hasMST       = false;
	bool          hasPrufer    = false;
	bool          isDirected   = false;

	void showKirchhoff(Graph<int> *g);
	void showKruskal(Graph<int> *g);
	void showPruferEncode();
	void showPruferDecode();
	void showMIS(Graph<int> *g);

	static Graph<int> buildMSTGraph(const vector<Edge> &edges, int n);

  public:
	void onNewGraph(bool directed);
	void processChoice(int choice, Graph<int> *g);
};
