#include "kruskal.h"
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;

Kruskal::Kruskal(const Graph<int> &g) : n(g.getNumVertices()) {
	auto adj = g.getAdjMatrix();
	auto wgt = g.getWeightMatrix();
	// берём каждое ребро один раз (верхний треугольник — для неориентированного)
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (adj[i][j] != 0)
				edges.push_back({i, j, abs(wgt[i][j])});
}

int Kruskal::find(int x) {
	if (parent[x] != x)
		parent[x] = find(parent[x]);
	return parent[x];
}

bool Kruskal::unite(int u, int v) {
	u = find(u);
	v = find(v);
	if (u == v) return false;
	if (rnk[u] < rnk[v]) swap(u, v);
	parent[v] = u;
	if (rnk[u] == rnk[v]) rnk[u]++;
	return true;
}

/*
    Псевдокод из задания (адаптирован к 0-индексации):
      T := ∅
      for i from 1 to n-1:
        while z(T + E[k]) > 0: k++   (цикл → пропустить)
        T := T + E[k]; k++
*/
vector<Edge> Kruskal::compute() {
	sort(edges.begin(), edges.end());

	parent.resize(n);
	rnk.assign(n, 0);
	iota(parent.begin(), parent.end(), 0);

	vector<Edge> mst;
	for (auto &e : edges) {
		if (unite(e.u, e.v)) {
			mst.push_back(e);
			if ((int)mst.size() == n - 1)
				break;
		}
	}
	return mst;
}

void Kruskal::printMST(const vector<Edge> &mst) {
	int total = 0;
	cout << "\n  Рёбра минимального остова:\n";
	for (auto &e : mst) {
		cout << "    " << e.u << " -- " << e.v << "  (вес: " << e.w << ")\n";
		total += e.w;
	}
	cout << "  Суммарный вес: " << total << "\n";
}
