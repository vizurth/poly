#include "cycle_basis.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
using namespace std;

/*
    LOOK: pathInMST(int src, int dst, const vector<Edge> &mst) const
    Через BFS находим путь от src до dst в MST. Если пути нет, возвращаем пустой вектор.
*/
vector<int> CycleBasis::pathInMST(int src, int dst,
                                  const vector<Edge> &mst) const {
	vector<vector<int>> adj(n);
	for (auto &e : mst) {
		adj[e.u].push_back(e.v);
		adj[e.v].push_back(e.u);
	}

	vector<int> parent(n, -1);
	vector<bool> visited(n, false);
	queue<int> q;
	q.push(src);
	visited[src] = true;

	while (!q.empty()) {
		int u = q.front();
		q.pop();
		if (u == dst)
			break;
		for (int v : adj[u]) {
			if (!visited[v]) {
				visited[v] = true;
				parent[v] = u;
				q.push(v);
			}
		}
	}

	if (!visited[dst])
		return {};

	vector<int> path;
	for (int v = dst; v != -1; v = parent[v])
		path.push_back(v);
	reverse(path.begin(), path.end());
	return path;
}

/*
    LOOK: symDiff(const EdgeSet &a, const EdgeSet &b)
	Считаем симметрическую разность множеств рёбер a и b. Результат - множество рёбер, которые есть в одном из множеств, но нет в другом.
*/
EdgeSet CycleBasis::symDiff(const EdgeSet &a, const EdgeSet &b) {
	EdgeSet result;
	for (auto &e : a) {
		if (!b.count(e)) {
			result.insert(e);
		}
	}

	for (auto &e : b) {
		if (!a.count(e)) {
			result.insert(e);
		}
	}
	return result;
}

/*
    LOOK: getBasis() const
    Получаем фундаментальную систему циклов.
*/
const vector<EdgeSet> &CycleBasis::getBasis() const { return basis; }

/*
    LOOK: compute(const vector<Edge> &mst, const Graph<int> &g)
    Строим ФСЦ на основе MST. Ищем хорды (рёбра, не входящие в MST) и для каждой хорды находим путь в MST между её концами. Этот путь + хорда образуют цикл.
*/
void CycleBasis::compute(const vector<Edge> &mst, const Graph<int> &g) {
	n = g.getNumVertices();
	basis.clear();

	vector<vector<bool>> inTree(n, vector<bool>(n, false));
	for (auto &e : mst) {
		inTree[e.u][e.v] = true;
		inTree[e.v][e.u] = true;
	}

	auto adj = g.getAdjMatrix();

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] == 0 || inTree[i][j]) {
				continue;
			}

			auto path = pathInMST(i, j, mst);
			if (path.empty()) {
				continue;
			}

			EdgeSet cycle;
			for (int k = 0; k + 1 < (int)path.size(); k++) {
				int u = min(path[k], path[k + 1]);
				int v = max(path[k], path[k + 1]);
				cycle.insert({u, v});
			}
			cycle.insert({i, j});
			basis.push_back(cycle);
		}
	}
}

/*
    LOOK: printBasis() const
    Выводит фундаментальную систему циклов.
*/
void CycleBasis::printBasis() const {
	cout << "\n  Фундаментальная система циклов (" << basis.size()
	     << " цикл(ов)):\n";
	if (basis.empty()) {
		cout << "  (граф является деревом)\n";
		return;
	}
	for (size_t i = 0; i < basis.size(); i++) {
		cout << "  [" << i + 1 << "] { ";
		for (auto &[u, v] : basis[i]) {
			cout << u << "-" << v << " ";
		}
		cout << "}\n";
	}
}
