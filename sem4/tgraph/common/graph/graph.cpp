#include "graph.h"
#include "../utils/utils.h"
#include <iomanip>
#include <iostream>

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