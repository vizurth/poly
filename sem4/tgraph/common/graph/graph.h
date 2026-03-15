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
	int getNumVertices() const;
	vector<vector<T>> getAdjMatrix() const;

	vector<int> bfs(int start) const;
	void dfs(int current, int target, vector<bool> &visited,
	         vector<int> &currentPath, vector<vector<int>> &allPaths) const;

	int eccentricity(int vertex) const;
	vector<int> allEccentricities() const;
	vector<int> findCenter() const;
	vector<int> findDiametral() const;

	vector<vector<int>> findAllPaths(int from, int to) const;
};

#include "graph.cpp"