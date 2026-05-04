#pragma once

#include "common/graph/graph.h"
#include <vector>
using namespace std;

struct Edge {
	int u, v, w;
	bool operator<(const Edge &o) const { return w < o.w; }
};

/*
    Алгоритм Краскала — минимальный остов.
    Сортирует рёбра по весу и жадно добавляет рёбра,
    не образующие цикл (проверка через DSU).
*/
class Kruskal {
  private:
	int n;
	vector<Edge> edges;

	// Вспомогательная функция для проверки пути в текущем остове (BFS)
	bool hasPath(int start, int target, const Graph<int> &tree) const;

  public:
	explicit Kruskal(const Graph<int> &g);

	vector<Edge> compute();

	static void printMST(const vector<Edge> &mst);
};
