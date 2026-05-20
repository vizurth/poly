#pragma once

#include <vector>

using namespace std;

template <typename T>
class Graph {
  private:
	int numVertices;
	vector<vector<int>> adjMatrix;
	vector<vector<T>> weightMatrix;

  public:
	Graph(int n);
	void addEdge(int from, int to, T weight);
	void removeEdge(int from, int to);
	bool hasEdge(int from, int to) const;

	void printAdjMatrix() const;
	void printWeightMatrix() const;
	void printEdges() const;

	T getEdge(int from, int to) const;
	int getNumVertices() const;
	vector<vector<int>> getAdjMatrix() const;
	vector<vector<T>> getWeightMatrix() const;
	int getEdgeCount() const;

	vector<int> bfs(int start) const;
	void dfs(int current, int target, vector<bool> &visited,
	         vector<int> &currentPath, vector<vector<int>> &allPaths) const;

	int eccentricity(int vertex) const;
	vector<int> allEccentricities() const;
	vector<int> findCenter() const;
	vector<int> findDiametral() const;

	vector<vector<int>> findAllPaths(int from, int to) const;

	Graph<T> reorder(const vector<int> &order) const;
};

#include "graph.cpp"