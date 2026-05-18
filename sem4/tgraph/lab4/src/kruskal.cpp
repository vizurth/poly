#include "kruskal.h"
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

/*
    LOOK: Kruskal(const Graph<int> &g)
    Конструктор класса Kruskal, который инициализирует список рёбер на основе
   входного графа. Рёбра хранятся в виде структуры Edge, которая содержит две
   вершины и вес ребра. Сложность: O(n^2) для полного графа.
*/
Kruskal::Kruskal(const Graph<int> &g) : n(g.getNumVertices()) {
	auto adj = g.getAdjMatrix();
	auto wgt = g.getWeightMatrix();
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (adj[i][j] != 0)
				edges.push_back({i, j, abs(wgt[i][j])});
}

/*
    LOOK: hasPath(int start, int target, const Graph<int> &tree)
    Проверяем наличие пути между двумя вершинами в текущем остове с помощью BFS
   по матрице смежности.
*/
bool Kruskal::hasPath(int start, int target, const Graph<int> &tree) const {
	if (start == target)
		return true;

	auto adj = tree.getAdjMatrix();
	vector<bool> visited(n, false);
	queue<int> q;

	q.push(start);
	visited[start] = true;

	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		for (int neighbor = 0; neighbor < n; neighbor++) {
			if (adj[curr][neighbor] == 0)
				continue;
			if (neighbor == target)
				return true;
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				q.push(neighbor); 
			}
		}
	}
	return false;
}

/*
    LOOK: compute()
    Основная функция для вычисления минимального остовного дерева. Сначала
   сортируем рёбра по весу, затем проходим по отсортированному списку рёбер и
   добавляем ребро в остов, если оно не создаёт цикл (проверяем с помощью
   hasPath)
*/
vector<Edge> Kruskal::compute() {
	sort(edges.begin(), edges.end());

	vector<Edge> T;
	Graph<int> tree(n);

	for (auto &e : edges) {
		if ((int)T.size() == n - 1)
			break;
		if (hasPath(e.u, e.v, tree))
			continue;	
		T.push_back(e);
		tree.addEdge(e.u, e.v, e.w);
		tree.addEdge(e.v, e.u, e.w);
	}

	if ((int)T.size() != n - 1)
		return {};

	return T;
}

/*
    LOOK: printMST(const vector<Edge> &mst)
    Вывод рёбер минимального остовного дерева на экран. Также выводим суммарный
   вес остова.
*/
void Kruskal::printMST(const vector<Edge> &mst) {
	int total = 0;
	cout << "\n  Рёбра минимального остова:\n";
	for (auto &e : mst) {
		cout << "    " << e.u << " -- " << e.v << "  (вес: " << e.w << ")\n";
		total += e.w;
	}
	cout << "  Суммарный вес: " << total << "\n";
}
