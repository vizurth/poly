#pragma once

#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

namespace Lab5Traversal {

inline int degree(const vector<vector<int>> &adj, int v) {
	int d = 0;
	for (int to = 0; to < (int)adj.size(); to++)
		if (to != v && adj[v][to] != 0)
			d++;
	return d;
}

inline vector<int> bfsPath(const vector<vector<int>> &adj, int from, int to) {
	int n = (int)adj.size();
	vector<int> parent(n, -1);
	vector<bool> visited(n, false);
	queue<int> q;

	visited[from] = true;
	q.push(from);

	while (!q.empty()) {
		int v = q.front();
		q.pop();
		if (v == to)
			break;

		for (int nxt = 0; nxt < n; nxt++) {
			if (adj[v][nxt] == 0 || visited[nxt])
				continue;
			visited[nxt] = true;
			parent[nxt] = v;
			q.push(nxt);
		}
	}

	if (!visited[to])
		return {};

	vector<int> path;
	for (int cur = to; cur != -1; cur = parent[cur])
		path.push_back(cur);
	reverse(path.begin(), path.end());
	return path;
}

inline void dfsComponent(int v, const vector<vector<int>> &adj,
                         vector<bool> &used, vector<int> &component) {
	used[v] = true;
	component.push_back(v);

	for (int to = 0; to < (int)adj.size(); to++) {
		if (adj[v][to] == 0 || used[to])
			continue;
		dfsComponent(to, adj, used, component);
	}
}

inline vector<vector<int>> components(const vector<vector<int>> &adj,
                                      bool skipIsolated = true) {
	int n = (int)adj.size();
	vector<bool> used(n, false);
	vector<vector<int>> result;

	for (int start = 0; start < n; start++) {
		if (used[start])
			continue;
		if (skipIsolated && degree(adj, start) == 0)
			continue;

		vector<int> component;
		dfsComponent(start, adj, used, component);
		result.push_back(component);
	}

	return result;
}

} // namespace Lab5Traversal
