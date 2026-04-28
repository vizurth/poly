#include "graph.h"
#include "../utils/utils.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;

/*
    LOOK: Graph(int n)
    Конструктор графа
*/
template <typename T>
Graph<T>::Graph(int n)
    : numVertices(n), adjMatrix(n, vector<int>(n, 0)),
      weightMatrix(n, vector<T>(n, 0)) {}

/*
    LOOK: void addEdge(int from, int to, T weight)
    Добавляем рёбра в граф
*/

template <typename T>
void Graph<T>::addEdge(int from, int to, T weight) {
	if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
		adjMatrix[from][to] = 1;
		weightMatrix[from][to] = weight;
	}
}

/*
    LOOK: bool hasEdge(int from, int to) const
    Проверяем есть ли рёбра между вершинами
*/
template <typename T>
bool Graph<T>::hasEdge(int from, int to) const {
	if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
		return adjMatrix[from][to] == 1;
	}

	return false;
}

/*
    LOOK: void printAdjMatrix() const;
    Выводим матрицу смежности
*/

template <typename T>
void Graph<T>::printAdjMatrix() const {
	printMatrix(adjMatrix, "Матрица смежности:");
}

/*
    LOOK: void printWeightMatrix() const;
    Выводим матрицу весов
*/

template <typename T>
void Graph<T>::printWeightMatrix() const {
	printMatrix(weightMatrix, "Матрица весов:");
}

/*
    LOOK: void printEdges() const;
    Выводим список рёбер
*/
template <typename T>
void Graph<T>::printEdges() const {
	cout << "Рёбра графа:\n";
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			if (adjMatrix[i][j] == 1) {
				cout << "  " << i << " -> " << j
				     << " (вес: " << weightMatrix[i][j] << ")\n";
			}
		}
	}
}

/*
    LOOK: T getEdge(int from, int to) const;
    Получаем вес рёбра между вершинами
*/
template <typename T>
T Graph<T>::getEdge(int from, int to) const {
	if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
		return weightMatrix[from][to];
	}
	return T{};
}

/*
    LOOK: int getNumVertices() const;
    Получаем количество вершин в графе
*/
template <typename T>
int Graph<T>::getNumVertices() const {
	return numVertices;
}

/*
    LOOK: vector<vector<T>> getAdjMatrix() const;
    Получаем матрицу смежности графа
*/
template <typename T>
vector<vector<int>> Graph<T>::getAdjMatrix() const {
	return adjMatrix;
}

/*
    LOOK: vector<vector<T>> getWeightMatrix() const;
    Получаем матрицу смежности графа
*/
template <typename T>
vector<vector<T>> Graph<T>::getWeightMatrix() const {
	return weightMatrix;
}

/*
    LOOK: vector<int> bfs(int start) const;
    BFS для поиска кратчайших путей от вершины start
*/
template <typename T>
vector<int> Graph<T>::bfs(int start) const {
	const int INF = std::numeric_limits<int>::max();
	vector<int> distances(numVertices, INF);
	distances[start] = 0;

	queue<int> q;
	q.push(start);

	while (!q.empty()) {
		int v = q.front();
		q.pop();

		for (int to = 0; to < numVertices; to++) {
			if (weightMatrix[v][to] != T{} && distances[to] == INF) {
				distances[to] = distances[v] + 1;
				q.push(to);
			}
		}
	}

	return distances;
}

/*
    LOOK: void eccentricity(int vertex) const;
    Вычисляем эксцентриситет вершины
*/
template <typename T>
int Graph<T>::eccentricity(int vertex) const {
	vector<int> dist = bfs(vertex);
	const int INF = std::numeric_limits<int>::max();
	

	int maxDist = 0;
	for (int i = 0; i < numVertices; i++) {
		if (i == vertex)
			continue;
		if (dist[i] == INF)
			continue;
		maxDist = std::max(maxDist, dist[i]);
	}

	return maxDist;
}

/*
    LOOK: void allEccentricities() const;
    Вычисляем эксцентриситеты для всех вершин
*/
template <typename T>
vector<int> Graph<T>::allEccentricities() const {
	vector<int> eccs(numVertices);
	for (int i = 0; i < numVertices; i++) {
		eccs[i] = eccentricity(i);
	}
	return eccs;
}

/*
    LOOK: vector<int> findCenter() const;
    Находим центр графа, вершины с минимальным эксцентриситетом
*/
template <typename T>
vector<int> Graph<T>::findCenter() const {
	vector<int> eccs = allEccentricities();
	const int INF = std::numeric_limits<int>::max();

	int radius = INF;
	for (int ecc : eccs) {
		if (ecc < radius) {
			radius = ecc;
		}
	}

	vector<int> centers;
	for (int i = 0; i < numVertices; i++) {
		if (eccs[i] == radius) {
			centers.push_back(i);
		}
	}
	return centers;
}

/*
    LOOK: vector<int> findDiametral() const;
    Находим периферийные вершины, у которых эксцентриситет равен диаметру
*/
template <typename T>
vector<int> Graph<T>::findDiametral() const {
	vector<int> eccs = allEccentricities();
	const int INF = std::numeric_limits<int>::max();

	int diameter = 0;
	for (int ecc : eccs) {
		if (ecc != INF && ecc > diameter) {
			diameter = ecc;
		}
	}

	vector<int> diametral;
	for (int i = 0; i < numVertices; i++) {
		if (eccs[i] == diameter) {
			diametral.push_back(i);
		}
	}
	return diametral;
}

/*
    LOOK: dfs(int current, int target, vector<bool>& visited, vector<int>&
   currentPath, vector<vector<int>>& allPaths) const; DFS ищем все пути от
   current до target
*/
template <typename T>
void Graph<T>::dfs(int current, int target, vector<bool> &visited,
                   vector<int> &currentPath,
                   vector<vector<int>> &allPaths) const {
	// достигли цели — сохраняем путь
	if (current == target) {
		allPaths.push_back(currentPath);
		return;
	}

	// обходим всех соседей
	for (int next = 0; next < numVertices; next++) {
		if (weightMatrix[current][next] != T{} && !visited[next]) {
			visited[next] = true;
			currentPath.push_back(next);

			dfs(next, target, visited, currentPath, allPaths);

			currentPath.pop_back();
			visited[next] = false;
		}
	}
}

/*
    LOOK: findAllPaths(int from, int to)
    Находим все пути от вершины from до вершины to
*/
template <typename T>
vector<vector<int>> Graph<T>::findAllPaths(int from, int to) const {
	vector<vector<int>> allPaths;
	vector<bool> visited(numVertices, false);
	vector<int> currentPath = {from}; // путь начинается с from

	visited[from] = true;
	dfs(from, to, visited, currentPath, allPaths);

	return allPaths;
}

/*
    LOOK: Graph<T> reorder(const vector<int>& order) const
    Меняем порядок вершин в графе в соответствии с переданной перестановкой order
*/
template <typename T>
Graph<T> Graph<T>::reorder(const vector<int> &order) const {
	Graph<T> result(numVertices);
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			if (adjMatrix[i][j] != 0) {
				result.addEdge(order[i], order[j], weightMatrix[i][j]);
			}
		}
	}
	return result;
}