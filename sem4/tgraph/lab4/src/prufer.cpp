#include "prufer.h"
#include <iostream>
using namespace std;

/*
	LOOK: encode(const vector<Edge> &mst, int p)
	Преобразуем минимальное остовное дерево в код Прюфера.
*/
PruferCode Prufer::encode(const vector<Edge> &mst, int p) {
	PruferCode code;
	code.n = p;

	vector<vector<pair<int, int>>> adj(p);
	vector<int> d(p, 0);
	for (auto &e : mst) {
		adj[e.u].push_back({e.v, e.w});
		adj[e.v].push_back({e.u, e.w});
		d[e.u]++;
		d[e.v]++;
	}

	vector<bool> V(p, true);

	for (int i = 0; i < p - 1; i++) {
		int v = -1;
		for (int k = 0; k < p; k++) {
			if (V[k] && d[k] == 1) {
				v = k;
				break;
			}
		}

		int neighbor = -1;
		int weight = 0;
		for (auto &edge : adj[v]) {
			if (V[edge.first]) {
				neighbor = edge.first;
				weight = edge.second;
				break;
			}
		}

		code.seq.push_back(neighbor);  
		code.weights.push_back(weight);

		V[v] = false;
		d[v]--;
		d[neighbor]--;
	}

	return code;
}

/*
	LOOK: decode(const PruferCode &code)
	Преобразуем код Прюфера обратно в множество рёбер.
*/
vector<Edge> Prufer::decode(const PruferCode &code) {
	int p = code.n;

	vector<Edge> E;

	vector<bool> B(p, true);

	for (int i = 0; i < p - 1; i++) {
		// Ищем минимальную неиспользованную вершину, которой НЕТ в остатке кода
		int v = -1;
		for (int k = 0; k < p; k++) {
			if (B[k]) {
				bool in_rest_of_code = false;
				for (int j = i; j < p - 1; j++) {
					if (code.seq[j] == k) {
						in_rest_of_code = true;
						break;
					}
				}
				if (!in_rest_of_code) { // если не нашли в остатке, добавляем в код
					v = k;
					break;
				}
			}
		}

		E.push_back({v, code.seq[i], code.weights[i]});

		B[v] = false;
	}

	return E;
}

/*
	LOOK: printCode(const PruferCode &code)
	Выводим код Прюфера на экран. Показываем как последовательность вершин, так и соответствующие веса рёбер для удобства.
*/
void Prufer::printCode(const PruferCode &code) {
	cout << "\n  Код Прюфера (" << code.seq.size() << " элементов):\n";
	cout << "  Последовательность: [ ";
	for (int v : code.seq)
		cout << v << " ";
	cout << "]\n";
	cout << "  Веса рёбер:         [ ";
	for (int w : code.weights)
		cout << w << " ";
	cout << "]\n";
}

/*
	LOOK: printEdges(const vector<Edge> &edges, const string &title)
	Выводим множество рёбер на экран. Также выводим суммарный вес рёбер для удобства.
*/
void Prufer::printEdges(const vector<Edge> &edges, const string &title) {
	cout << "\n  " << title << "\n";
	int total = 0;
	for (auto &e : edges) {
		cout << "    " << e.u << " -- " << e.v << "  (вес: " << e.w << ")\n";
		total += e.w;
	}
	cout << "  Суммарный вес: " << total << "\n";
}