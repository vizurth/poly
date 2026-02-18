#pragma once

#include <vector>

using namespace std;

// (u, v) == (from, to) - ориентированный граф, рёбра хранятся в виде матрицы смежности

template <typename T>
class Graph {
private:
	int numVertices;
	vector<vector<T>> adjMatrix; // матрица смежности

public:
	Graph(int n) : numVertices(n), adjMatrix(n, vector<T>(n, 0)) {}
	void addEdge(int u, int v, T weight);

	bool hasEdge(int u, int v) const;
	
	void printAdjMatrix() const;
	void printEdges() const; // выводим списком

	// геттеры
	int getNumVertices() const;
	T getEdge(int u, int v) const;
	const vector<vector<T>>& getAdjMatrix() const;
	vector<pair<int, T>> getOutgoingEdges(int u) const;
	vector<pair<int, T>> getIncomingEdges(int v) const;
	vector<pair<int, T>> getAdj(int v) const; // список соседей для не ор графа
};

#include "graph.cpp"