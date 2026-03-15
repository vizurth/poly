#include "fulkerson.h"

template <typename T>
Fulkerson<T>::Fulkerson(const Graph<T> &g) : graph(g), n(g.getNumVertices()) {}

/*
    LOOK: findRoots
    Шаг 1 и 2: ищем вершины без входящих дуг
    среди ещё не вычеркнутых вершин
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
			// учитываем только не вычеркнутые вершины
			if (!removed[from] && adj[from][v] != T{}) {
				hasIncoming = true;
				break;
			}
		}

		if (!hasIncoming) {
			roots.push_back(v);
		}
	}

	return roots;
}

/*
    LOOK: computeOrder
    Основной алгоритм Фалкерсона

    Возвращает вектор order где:
    order[старый_номер] = новый_номер

    Группы выводятся пошагово:
    Группа 1 — вершины без входящих дуг в исходном графе
    Группа 2 — вершины без входящих дуг после удаления группы 1
    и т.д.
*/
template <typename T>
vector<int> Fulkerson<T>::computeOrder() const {
	vector<bool> removed(n, false);
	vector<int> order(n, -1);
	int counter = 0;

	while (counter < n) {
		// Шаг 1/2: находим вершины без входящих дуг
		vector<int> roots = findRoots(removed);

		if (roots.empty()) {
			cout << "  Ошибка: граф содержит цикл!\n";
			cout << "  Алгоритм Фалкерсона применим только к DAG\n";
			return {};
		}

		// Присваиваем очередные номера вершинам группы
		for (int v : roots) {
			order[v] = counter++;
			removed[v] = true; // вычёркиваем вершину и её дуги
		}
	}

	return order;
}

/*
    LOOK: printResult
    Выводим результат — группы и новую нумерацию
*/
template <typename T>
void Fulkerson<T>::printResult(const vector<int> &order) const {
	if (order.empty())
		return;

	// Группируем вершины по новому номеру (восстанавливаем группы)
	// order[v] — номер вершины v в новом порядке
	// Группа — вершины с одинаковой "волной" нумерации
	// Восстанавливаем через повторный запуск без side effects
	vector<bool> removed(n, false);
	int groupNum = 1;
	int counter = 0;

	cout << "\n── Алгоритм Фалкерсона ─────────────────\n";

	while (counter < n) {
		vector<int> roots = findRoots(removed);

		cout << "  Группа " << groupNum++ << ": { ";
		for (int v : roots) {
			cout << v << " → " << order[v] << "  ";
			removed[v] = true;
			counter++;
		}
		cout << "}\n";
	}

	// Выводим итоговую нумерацию
	cout << "\n  Итоговая нумерация вершин:\n";
	cout << "  Старый номер: ";
	for (int v = 0; v < n; v++)
		cout << setw(4) << v;
	cout << "\n";
	cout << "  Новый номер:  ";
	for (int v = 0; v < n; v++)
		cout << setw(4) << order[v];
	cout << "\n";

	// Выводим новую матрицу смежности
	auto adj = graph.getAdjMatrix();
	vector<vector<T>> newAdj(n, vector<T>(n, T{}));
	for (int from = 0; from < n; from++) {
		for (int to = 0; to < n; to++) {
			if (adj[from][to] != T{}) {
				newAdj[order[from]][order[to]] = adj[from][to];
			}
		}
	}

	printMatrix(newAdj, "Матрица смежности после нумерации Фалкерсона:");
}