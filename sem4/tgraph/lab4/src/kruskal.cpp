#include "kruskal.h"
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

/*
	LOOK: Kruskal(const Graph<int> &g)
	Конструктор класса Kruskal, который инициализирует список рёбер на основе входного графа. Рёбра хранятся в виде структуры Edge, которая содержит две вершины и вес ребра. Сложность: O(n^2) для полного графа.
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
	LOOK: hasPath(int start, int target, const vector<vector<int>> &adjT)
	Вспомогательная функция для проверки наличия пути между двумя вершинами в текущем остове. Используем алгоритм BFS для обхода графа. Сложность: O(n + m), n - количество вершин, m - количество рёбер в остове.
*/
bool Kruskal::hasPath(int start, int target,
                      const vector<vector<int>> &adjT) const {
	if (start == target)
		return true;

	vector<bool> visited(n, false);
	queue<int> q;

	q.push(start);
	visited[start] = true;

	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		for (int neighbor : adjT[curr]) {
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
	Основная функция для вычисления минимального остовного дерева. Сначала сортируем рёбра по весу, затем проходим по отсортированному списку рёбер и добавляем ребро в остов, если оно не создаёт цикл (проверяем с помощью hasPath). Сложность: O(E log E) из-за сортировки, где E - количество рёбер.
*/
vector<Edge> Kruskal::compute() {
	sort(edges.begin(), edges.end());

	vector<Edge> T; // T - множество рёбер остова
	vector<vector<int>> adjT(n);

	int k = 0;
	int total_edges = edges.size();

	for (int i = 0; i < n - 1 && k < total_edges;) {

		while (k < total_edges && hasPath(edges[k].u, edges[k].v, adjT)) {
			k++; // ищем следующее ребро, которое не создаёт цикл
		}

		if (k >= total_edges)
			break; // рёбра закончились

		T.push_back(edges[k]);

		adjT[edges[k].u].push_back(edges[k].v);
		adjT[edges[k].v].push_back(edges[k].u);

		k++;

		i++;
	}

	return T;
}

/*
	LOOK: printMST(const vector<Edge> &mst)
	Вывод рёбер минимального остовного дерева на экран. Также выводим суммарный вес остова.
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