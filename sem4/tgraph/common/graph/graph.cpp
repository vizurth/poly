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
Graph<T>::Graph(int n) : numVertices(n), adjMatrix(n, vector<T>(n, 0)) {}

/*
    LOOK: void addEdge(int from, int to, T weight)
    Добавляем рёбра в граф
*/

template <typename T>
void Graph<T>::addEdge(int from, int to, T weight) {
	if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
		adjMatrix[from][to] = weight;
	}
}

/*
    LOOK: bool hasEdge(int from, int to) const
    Проверяем есть ли рёбра между вершинами
*/
template <typename T>
bool Graph<T>::hasEdge(int from, int to) const {
	if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
		return adjMatrix[from][to] != T{};
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
    LOOK: void printEdges() const;
    Выводим список рёбер
*/
template <typename T>
void Graph<T>::printEdges() const {
	cout << "Рёбра графа:\n";
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			if (adjMatrix[i][j] != T{}) {
				cout << "  " << i << " -> " << j << " (вес: " << adjMatrix[i][j]
				     << ")\n";
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
		return adjMatrix[from][to];
	}
	return T{};
}

/*
    LOOK: vector<pair<int, T>> getOutgoingEdges(int vertex) const;
    Получаем список рёбер, исходящих из вершины
*/
template <typename T>
vector<pair<int, T>> Graph<T>::getOutgoingEdges(int vertex) const {
	vector<pair<int, T>> result;

	if (vertex >= 0 && vertex < numVertices) {
		for (int i = 0; i < numVertices; i++) {
			if (adjMatrix[vertex][i] != T{}) {
				result.push_back({i, adjMatrix[vertex][i]});
			}
		}
	}

	return result;
}

/*
    LOOK: vector<pair<int, T>> getIncomingEdges(int vertex) const;
    Получаем список рёбер, входящих в вершину
*/
template <typename T>
vector<pair<int, T>> Graph<T>::getIncomingEdges(int vertex) const {
	vector<pair<int, T>> result;

	if (vertex >= 0 && vertex < numVertices) {
		for (int i = 0; i < numVertices; i++) {
			if (adjMatrix[i][vertex] != T{}) {
				result.push_back({i, adjMatrix[i][vertex]});
			}
		}
	}

	return result;
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
vector<vector<T>> Graph<T>::getAdjMatrix() const {
	return adjMatrix;
}

template <typename T>
vector<int> Graph<T>::bfs(int start) const {
	const int INF = std::numeric_limits<int>::max(); // ← int, не T!
	vector<int> distances(numVertices, INF);
	distances[start] = 0;

	queue<int> q;
	q.push(start);

	while (!q.empty()) {
		int v = q.front();
		q.pop();

		for (int to = 0; to < numVertices; to++) {
			if (adjMatrix[v][to] != T{} && distances[to] == INF) {
				distances[to] = distances[v] + 1;
				q.push(to);
			}
		}
	}

	return distances;
}

template <typename T>
int Graph<T>::eccentricity(int vertex) const {
	vector<int> dist = bfs(vertex);
	const int INF = std::numeric_limits<int>::max();

	int maxDist = 0;
	for (int i = 0; i < numVertices; i++) {
		if (i == vertex)
			continue;
		if (dist[i] == INF)
			return INF; // вершина недостижима
		maxDist = std::max(maxDist, dist[i]);
	}

	return maxDist;
}

template <typename T>
vector<int> Graph<T>::allEccentricities() const {
	vector<int> eccs(numVertices);
	for (int i = 0; i < numVertices; i++) {
		eccs[i] = eccentricity(i);
	}
	return eccs;
}

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