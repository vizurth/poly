#include "prufer.h"
#include <iostream>
using namespace std;

/*
    Псевдокод кодирования (0-indexed, n-1 итераций):
      for i = 0..n-2:
        v := min висячая вершина
        A[i] := сосед(v)
        V := V - v
*/
PruferCode Prufer::encode(const vector<Edge> &mst, int n) {
	vector<int> degree(n, 0);
	vector<vector<pair<int, int>>> adj(n); // adj[v] = { (сосед, вес) }

	for (auto &e : mst) {
		adj[e.u].push_back({e.v, e.w});
		adj[e.v].push_back({e.u, e.w});
		degree[e.u]++;
		degree[e.v]++;
	}

	vector<bool> removed(n, false);
	PruferCode code;
	code.n = n;

	for (int i = 0; i < n - 1; i++) {
		// min висячая вершина
		int leaf = -1;
		for (int v = 0; v < n; v++)
			if (!removed[v] && degree[v] == 1) { leaf = v; break; }

		// найти соседа
		int neighbor = -1, weight = 0;
		for (auto [u, w] : adj[leaf])
			if (!removed[u]) { neighbor = u; weight = w; break; }

		code.seq.push_back(neighbor);
		code.weights.push_back(weight);

		removed[leaf] = true;
		degree[neighbor]--;
	}

	return code;
}

/*
    Псевдокод декодирования (0-indexed):
      count[v] := число вхождений v в code
      for i = 0..n-2:
        v := min вершина с count[v] == 0 и не использованная
        E := E + (v, code[i])
        используем v; count[code[i]]--
*/
vector<Edge> Prufer::decode(const PruferCode &code) {
	int n = code.n;
	vector<int> cnt(n, 0);
	for (int v : code.seq) cnt[v]++;

	vector<bool> used(n, false);
	vector<Edge> edges;

	for (int i = 0; i < (int)code.seq.size(); i++) {
		int v = -1;
		for (int k = 0; k < n; k++)
			if (!used[k] && cnt[k] == 0) { v = k; break; }

		edges.push_back({v, code.seq[i], code.weights[i]});
		used[v] = true;
		cnt[code.seq[i]]--;
	}

	return edges;
}

void Prufer::printCode(const PruferCode &code) {
	cout << "\n  Код Прюфера (" << code.seq.size() << " элементов):\n";
	cout << "  Последовательность: [ ";
	for (int v : code.seq) cout << v << " ";
	cout << "]\n";
	cout << "  Веса рёбер:         [ ";
	for (int w : code.weights) cout << w << " ";
	cout << "]\n";
}

void Prufer::printEdges(const vector<Edge> &edges, const string &title) {
	cout << "\n  " << title << "\n";
	int total = 0;
	for (auto &e : edges) {
		cout << "    " << e.u << " -- " << e.v << "  (вес: " << e.w << ")\n";
		total += e.w;
	}
	cout << "  Суммарный вес: " << total << "\n";
}
