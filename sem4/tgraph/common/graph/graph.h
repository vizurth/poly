#pragma once

#include <vector>

using namespace std;

// (u, v) == (from, to) - ориентированный граф, рёбра хранятся в виде матрицы смежности

template <typename T>
class Graph {
private:
	int numVertices; // количество вершин
	vector<vector<T>> adjMatrix; // матрица смежности

public:
	Graph(int n) : numVertices(n), adjMatrix(n, vector<T>(n, 0)) {} // конструктор: создаёт граф с n вершинами

	int getNumVertices() const; // получить количество вершин
	void addEdge(int u, int v, T weight); // добавить ориентированное ребро (u, v) с весом weight
	T getEdge(int u, int v) const; // получить вес ребра (u, v), 0 если ребра нет
	bool hasEdge(int u, int v) const; // проверить наличие ребра (u, v)

	const vector<vector<T>>& getAdjMatrix() const; // получить матрицу смежности
	
	void printAdjMatrix() const; // вывести матрицу смежности на экран
	void printEdges() const; // вывести список рёбер на экран

	vector<pair<int, T>> getOutgoingEdges(int u) const; // получить список рёбер, исходящих из вершины u
	vector<pair<int, T>> getIncomingEdges(int v) const; // получить список рёбер, входящих в вершину v
	vector<pair<int, T>> getAdj(int v) const; // получить список соседей вершины v (для неориентированного графа)
};