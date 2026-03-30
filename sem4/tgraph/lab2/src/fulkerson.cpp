#include "fulkerson.h"
#include <iomanip>
#include <iostream>
using namespace std;

template <typename T>
Fulkerson<T>::Fulkerson(const Graph<T> &g) : graph(g), n(g.getNumVertices()) {}

/*
    LOOK: findRoots
    Ищем вершины без входящих дуг среди ещё не вычеркнутых.
*/
template <typename T>
vector<int> Fulkerson<T>::findRoots(const vector<bool> &removed) const {
	auto adj = graph.getAdjMatrix();
	vector<int> roots;

	for (int v = 0; v < n; v++) {
		if (removed[v])
			continue;

		bool hasIncoming = false;
		for (int from = 0; from < n; from++) {
			iterCount++;

			if (!removed[from] && from != v && adj[from][v] != 0) {
				hasIncoming = true;
				break;
			}
		}

		if (!hasIncoming)
			roots.push_back(v);
	}

	return roots;
}

/*
    LOOK: computeOrder

    Шаги:
    1. Найти все вершины без входящих дуг (корни).
    2. Присвоить им очередные номера.
    3. Вычеркнуть их из графа.
    4. Повторять до обработки всех вершин.
    Если корней нет, граф содержит цикл и алгоритм неприменим.
*/
template <typename T>
vector<int> Fulkerson<T>::computeOrder() const {
	iterCount = 0;
	vector<bool> removed(n, false);
	vector<int> order(n, -1);
	int counter = 0;

	while (counter < n) {
		vector<int> roots = findRoots(removed);

		if (roots.empty()) {
			cout << "  Ошибка: граф содержит цикл!\n";
			cout << "  Алгоритм Фалкерсона применим только к DAG.\n";
			return {};
		}

		for (int v : roots) {
			order[v] = counter++;
			removed[v] = true;
		}
	}

	return order;
}

/*
    LOOK: printResult
    Выводим группы вершин, итоговую нумерацию и
    новую матрицу смежности после перенумерации.
*/
template <typename T>
void Fulkerson<T>::printResult(const vector<int> &order) const {
	if (order.empty())
		return;

	cout << "\n== Алгоритм Фалкерсона ==\n\n";

	vector<bool> removed(n, false);
	int groupNum = 1;
	int counter = 0;

	while (counter < n) {
		vector<int> roots = findRoots(removed);

		cout << "  Группа " << groupNum++ << ": { ";
		for (int v : roots) {
			cout << v << " -> " << order[v] << "  ";
			removed[v] = true;
			counter++;
		}
		cout << "}\n";
	}

	cout << "\n  Итоговая нумерация вершин:\n";
	cout << "  Старый номер: ";
	for (int v = 0; v < n; v++)
		cout << setw(4) << v;
	cout << "\n";
	cout << "  Новый номер:  ";
	for (int v = 0; v < n; v++)
		cout << setw(4) << order[v];
	cout << "\n";

	Graph<T> reordered = graph.reorder(order);

	cout << "\n  Матрица смежности после нумерации Фалкерсона:\n";
	reordered.printAdjMatrix();

	cout << "\n  Матрица весов после нумерации Фалкерсона:\n";
	reordered.printWeightMatrix();
}