#include "cycle_basis.h"
#include "traversal.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>

using namespace std;

CycleBasis::CycleBasis(const Graph<int> &g) : graph(g) {}

pair<int, int> CycleBasis::normalizeEdge(int u, int v) {
	if (u > v)
		swap(u, v);
	return {u, v};
}

vector<int> CycleBasis::buildEulerCycle(vector<vector<int>> &adj,
                                        int start) const {
	vector<int> cycle;
	stack<int> st;
	st.push(start);

	while (!st.empty()) {
		int v = st.top();
		int to = -1;

		for (int i = 0; i < (int)adj.size(); i++) {
			if (adj[v][i] != 0) {
				to = i;
				break;
			}
		}

		if (to == -1) {
			cycle.push_back(v);
			st.pop();
		} else {
			adj[v][to] = adj[to][v] = 0;
			st.push(to);
		}
	}

	reverse(cycle.begin(), cycle.end());
	return cycle;
}

bool CycleBasis::build() {
	Kruskal kr(graph);
	mstEdges = kr.compute();
	if (mstEdges.empty())
		return false;

	int n = graph.getNumVertices();
	mstAdj.assign(n, vector<int>(n, 0));
	for (auto &e : mstEdges)
		mstAdj[e.u][e.v] = mstAdj[e.v][e.u] = 1;

	fundamentalCycles.clear();
	auto adj = graph.getAdjMatrix();

	for (int u = 0; u < n; u++) {
		for (int v = u + 1; v < n; v++) {
			if (adj[u][v] == 0 && adj[v][u] == 0)
				continue;
			if (mstAdj[u][v] != 0 || mstAdj[v][u] != 0)
				continue;

			vector<int> path = Lab5Traversal::bfsPath(mstAdj, u, v);
			if (path.empty())
				continue;
			path.push_back(u);
			fundamentalCycles.push_back(path);
		}
	}

	return true;
}

const vector<Edge> &CycleBasis::getMSTEdges() const {
	return mstEdges;
}

const vector<vector<int>> &CycleBasis::getFundamentalCycles() const {
	return fundamentalCycles;
}

SymDiffResult CycleBasis::symmetricDifference(
    const vector<int> &indices1Based) const {
	map<pair<int, int>, int> parity;

	for (int idx1 : indices1Based) {
		int idx = idx1 - 1;
		if (idx < 0 || idx >= (int)fundamentalCycles.size())
			continue;

		const auto &cycle = fundamentalCycles[idx];
		for (int i = 0; i + 1 < (int)cycle.size(); i++)
			parity[normalizeEdge(cycle[i], cycle[i + 1])] ^= 1;
	}

	SymDiffResult result;
	for (auto &kv : parity)
		if (kv.second)
			result.edges.push_back(kv.first);

	vector<vector<int>> adj(graph.getNumVertices(),
	                        vector<int>(graph.getNumVertices(), 0));
	for (auto &e : result.edges)
		adj[e.first][e.second] = adj[e.second][e.first] = 1;

	auto comps = Lab5Traversal::components(adj, true);
	for (auto &c : comps) {
		vector<int> cycle = buildEulerCycle(adj, c[0]);
		if (cycle.size() > 1)
			result.contours.push_back(cycle);
	}

	return result;
}

void CycleBasis::printFundamentalCycles(const vector<vector<int>> &cycles) {
	cout << "\n  Фундаментальная система циклов:\n";
	if (cycles.empty()) {
		cout << "    отсутствует (граф совпадает с остовом)\n";
		return;
	}

	for (int i = 0; i < (int)cycles.size(); i++) {
		cout << "    C" << i + 1 << ": ";
		for (int j = 0; j < (int)cycles[i].size(); j++) {
			if (j > 0)
				cout << " -> ";
			cout << cycles[i][j];
		}
		cout << "\n";
	}
}

void CycleBasis::printSymDiffResult(const SymDiffResult &result) {
	cout << "\n  Результат симметрической разности:\n";
	if (result.edges.empty()) {
		cout << "    пустое множество рёбер\n";
		return;
	}

	cout << "    Рёбра: ";
	for (int i = 0; i < (int)result.edges.size(); i++) {
		if (i > 0)
			cout << ", ";
		cout << "(" << result.edges[i].first << "-" << result.edges[i].second
		     << ")";
	}
	cout << "\n";

	for (int i = 0; i < (int)result.contours.size(); i++) {
		cout << "    Контур " << i + 1 << ": ";
		for (int j = 0; j < (int)result.contours[i].size(); j++) {
			if (j > 0)
				cout << " -> ";
			cout << result.contours[i][j];
		}
		cout << "\n";
	}
}
