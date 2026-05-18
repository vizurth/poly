#include "cycle_basis.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
using namespace std;

/*
    LOOK: pathInMST(int src, int dst, const vector<Edge> &mst) const
    BFS-поиск пути от src до dst в остовном дереве.
    Возвращает последовательность вершин или пустой вектор, если пути нет.
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
    Симметрическая разность: рёбра, принадлежащие ровно одному из множеств.
*/
EdgeSet CycleBasis::symDiff(const EdgeSet &a, const EdgeSet &b) {
	EdgeSet result;
	for (auto &e : a)
		if (!b.count(e))
			result.insert(e);
	for (auto &e : b)
		if (!a.count(e))
			result.insert(e);
	return result;
}

/*
    LOOK: printEdgeSet(const EdgeSet &es, const string &title)
    Выводит множество рёбер на экран.
*/
void CycleBasis::printEdgeSet(const EdgeSet &es, const string &title) {
	cout << "\n  " << title << "\n";
	if (es.empty()) {
		cout << "    (пустое множество)\n";
		return;
	}
	for (auto &[u, v] : es)
		cout << "    " << u << " -- " << v << "\n";
}

/*
    LOOK: compute(const vector<Edge> &mst, const Graph<int> &g)
    Строит фундаментальную систему циклов.
    Для каждого нетребового ребра (не входящего в MST) находит единственный
    фундаментальный цикл: путь в MST между концами ребра + само это ребро.
*/
void CycleBasis::compute(const vector<Edge> &mst, const Graph<int> &g) {
	n = g.getNumVertices();
	basis.clear();

	set<pair<int, int>> treeEdges;
	for (auto &e : mst)
		treeEdges.insert({min(e.u, e.v), max(e.u, e.v)});

	auto adj = g.getAdjMatrix();

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] == 0)
				continue;
			if (treeEdges.count({i, j}))
				continue;

			// нетребовое ребро (i,j) — строим фундаментальный цикл
			auto path = pathInMST(i, j, mst);
			if (path.empty())
				continue;

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
		for (auto &[u, v] : basis[i])
			cout << u << "-" << v << " ";
		cout << "}\n";
	}
}

/*
    LOOK: interactiveSymDiff() const
    Пользователь вводит номера фундаментальных циклов, программа
    вычисляет их симметрическую разность и выводит результат.
*/
void CycleBasis::interactiveSymDiff() const {
	if (basis.empty()) {
		cout << "  Фундаментальная система циклов пуста.\n";
		return;
	}

	printBasis();
	cout << "\n  Введите номера циклов через пробел (например: 1 3), затем Enter:\n  > ";

	string line;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, line);

	istringstream iss(line);
	vector<int> indices;
	int idx;
	while (iss >> idx) {
		idx--;
		if (idx >= 0 && idx < (int)basis.size())
			indices.push_back(idx);
		else
			cout << "  Индекс " << idx + 1 << " вне диапазона — пропущен.\n";
	}

	if ((int)indices.size() < 2) {
		cout << "  Нужно выбрать хотя бы 2 цикла.\n";
		return;
	}

	EdgeSet result = basis[indices[0]];
	cout << "\n  Операция: C" << indices[0] + 1;
	for (size_t i = 1; i < indices.size(); i++) {
		result = symDiff(result, basis[indices[i]]);
		cout << " △ C" << indices[i] + 1;
	}

	printEdgeSet(result, "Результат симметрической разности:");
}
