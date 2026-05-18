#include "mis.h"
#include <iostream>
using namespace std;

MIS::MIS(const vector<vector<int>> &adjMatrix)
    : n(adjMatrix.size()), adj(adjMatrix) {
	m = 0;
}

/*
    LOOK: isIndependent(const vector<int> &S, int v)
    Проверяем, можно ли добавить вершину v к множеству S, не нарушая
   независимости. Проходим по всем вершинам в S и проверяем наличие рёбер между
   ними и v.
*/
bool MIS::isIndependent(const vector<int> &S, int v) const {
	for (int u : S) {
		if (adj[u][v] != 0 || adj[v][u] != 0) {
			return false; // множество S + v зависимое
		}
	}
	return true;
}

/*
    LOOK: backtrack(vector<int> S, vector<int> T)
    Рекурсивная функция для перебора всех подмножеств вершин. S - текущее
   независимое множество, T - кандидаты для добавления. Для каждой вершины v в T
   проверяем, можно ли её добавить к S. Если да, то создаём новое множество
   S_plus_v и новый список кандидатов newT, исключая соседей v. Рекурсивно
   вызываем backtrack для S_plus_v и newT.
*/
void MIS::backtrack(vector<int> S, vector<int> T) {
	for (int v : T) {

		if (isIndependent(S, v)) {
			vector<int> S_plus_v = S;
			S_plus_v.push_back(v);
			if ((int)S_plus_v.size() > m) {
				X = S_plus_v;
				m = S_plus_v.size();
			}
			vector<int> newT;
			for (int u : T) {
				if (u != v && adj[v][u] == 0 && adj[u][v] == 0) {
					newT.push_back(u);
				}
			}
			backtrack(S_plus_v, newT);
		}
	}
}

/*
    LOOK: compute()
    Инициализируем m и X, создаём список всех вершин V и вызываем backtrack для
   пустого множества S и полного списка кандидатов V. В итоге X будет содержать
   максимальное независимое множество.
*/
vector<int> MIS::compute() {
	m = 0;
	X.clear();
	vector<int> V;
	for (int i = 0; i < n; i++) {
		V.push_back(i);
	}
	backtrack({}, V);
	return X;
}

/*
    LOOK: printResult(const vector<int> &mis)
    Выводим результат - максимальное независимое множество. Также выводим его
   размер для удобства.
*/
void MIS::printResult(const vector<int> &mis) {
	cout << "\n  Максимальное независимое множество (" << mis.size()
	     << " вершин):\n";
	cout << "  Вершины: { ";
	for (int v : mis)
		cout << v << " ";
	cout << "}\n";
}