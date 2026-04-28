#include "mis.h"
#include <iostream>
using namespace std;

MIS::MIS(const Graph<int> &g) : n(g.getNumVertices()), adj(g.getAdjMatrix()) {}

bool MIS::isIndependent(const vector<int> &S, int v) const {
	for (int u : S)
		if (adj[u][v] != 0) return false;
	return true;
}

/*
    Строго по псевдокоду:
      for v ∈ T:
        if IS_INDEPENDENT(S, v): BT(S ∪ {v}, T \ Γ*(v))
        T := T \ {v}           ← v удаляется из T в любом случае
*/
void MIS::backtrack(vector<int> S, vector<int> T) {
	if (T.empty()) {
		if ((int)S.size() > (int)bestSet.size())
			bestSet = S;
		return;
	}

	while (!T.empty()) {
		int v = T[0];

		if (isIndependent(S, v)) {
			// новый T = T \ Γ*(v) = убираем v и всех его соседей
			vector<int> newT;
			for (int u : T)
				if (u != v && adj[v][u] == 0)
					newT.push_back(u);

			S.push_back(v);
			backtrack(S, newT);
			S.pop_back();
		}

		T.erase(T.begin()); // T := T \ {v}
	}
}

vector<int> MIS::compute() {
	bestSet.clear();
	vector<int> all;
	for (int i = 0; i < n; i++) all.push_back(i);
	backtrack({}, all);
	return bestSet;
}

void MIS::printResult(const vector<int> &mis) {
	cout << "\n  Максимальное независимое множество (" << mis.size() << " вершин):\n";
	cout << "  Вершины: { ";
	for (int v : mis) cout << v << " ";
	cout << "}\n";
}
