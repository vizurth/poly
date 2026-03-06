#pragma once

#include <vector>

using namespace std;

template <typename T>
class Graph {
  private:
	int numVertices;
	vector<vector<T>> adjMatrix;

  public:
	Graph(int n);
	void addEdge(int from, int to, T weight);
	bool hasEdge(int from, int to) const;

	void printAdjMatrix() const;
	void printEdges() const;

	T getEdge(int from, int to) const;
	vector<pair<int, T>> getOutgoingEdges(int vertex) const;
	vector<pair<int, T>> getIncomingEdges(int vertex) const;
	int getNumVertices() const;
	vector<vector<T>> getAdjMatrix() const;
};

#include "graph.cpp"