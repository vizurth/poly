#include "euler.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

/*
    LOOK: EulerianCycle(const Graph<int> &graph)
    Конструктор создаёт рабочую копию графа.
*/
EulerianCycle::EulerianCycle(const Graph<int> &graph)
    : g(graph), n(graph.getNumVertices()) {}

/*
    LOOK: degree(int v) const
    Возвращает степень вершины v (сумма строки в матрице смежности).
*/
int EulerianCycle::degree(int v) const {
	auto adj = g.getAdjMatrix();
	int d = 0;
	for (int j = 0; j < n; j++)
		d += adj[v][j];
	return d;
}

/*
    LOOK: oddDegreeVertices() const
    Возвращает список вершин с нечётной степенью.
*/
vector<int> EulerianCycle::oddDegreeVertices() const {
	vector<int> odd;
	for (int v = 0; v < n; v++)
		if (degree(v) % 2 != 0)
			odd.push_back(v);
	return odd;
}

/*
    LOOK: nonZeroComponents() const
    BFS-обход: возвращает связные компоненты, содержащие хотя бы одно ребро.
*/
vector<vector<int>> EulerianCycle::nonZeroComponents() const {
	auto adj = g.getAdjMatrix();
	vector<bool> visited(n, false);
	vector<vector<int>> comps;

	for (int start = 0; start < n; start++) {
		if (visited[start] || degree(start) == 0)
			continue;

		vector<int> comp;
		queue<int> q;
		q.push(start);
		visited[start] = true;

		while (!q.empty()) {
			int u = q.front();
			q.pop();
			comp.push_back(u);
			for (int v = 0; v < n; v++) {
				if (!visited[v] && adj[u][v] != 0) {
					visited[v] = true;
					q.push(v);
				}
			}
		}
		comps.push_back(std::move(comp));
	}
	return comps;
}

/*
    LOOK: isEulerian() const
    Граф эйлеров: все вершины чётной степени и граф связный.
*/
bool EulerianCycle::isEulerian() const {
	return oddDegreeVertices().empty() && nonZeroComponents().size() <= 1;
}

/*
    LOOK: isSemiEulerian() const
    Граф полуэйлеров: ровно 2 вершины нечётной степени и граф связный.
*/
bool EulerianCycle::isSemiEulerian() const {
	return oddDegreeVertices().size() == 2 && nonZeroComponents().size() <= 1;
}

/*
    LOOK: makeEulerian()
    Добавляет рёбра для преобразования графа в эйлеров.
    Шаг 1: соединяет несвязные компоненты.
    Шаг 2: устраняет вершины нечётной степени попарным добавлением рёбер.
*/
void EulerianCycle::makeEulerian() {
	addedEdges.clear();

	// Шаг 1: соединяем несвязные компоненты
	auto comps = nonZeroComponents();
	cout << "  Компонент с рёбрами: " << comps.size() << "\n";
	for (size_t i = 1; i < comps.size(); i++) {
		int u = comps[i - 1].front();
		int v = comps[i].front();
		g.addEdge(u, v, 1);
		g.addEdge(v, u, 1);
		addedEdges.emplace_back(u, v);
		cout << "  [+] Ребро " << u << " -- " << v << " (соединение компонент)\n";
	}

	// Шаг 2: устраняем вершины нечётной степени
	for (int iter = 0; iter < 64; iter++) {
		auto odd = oddDegreeVertices();
		if (odd.empty())
			break;

		cout << "  Вершин нечётной степени: " << odd.size() << " [";
		for (size_t i = 0; i < odd.size(); i++) {
			if (i) cout << ", ";
			cout << odd[i];
		}
		cout << "]\n";

		bool added = false;
		for (size_t i = 0; i < odd.size() && !added; i++) {
			for (size_t j = i + 1; j < odd.size() && !added; j++) {
				if (!g.hasEdge(odd[i], odd[j])) {
					g.addEdge(odd[i], odd[j], 1);
					g.addEdge(odd[j], odd[i], 1);
					addedEdges.emplace_back(odd[i], odd[j]);
					cout << "  [+] Ребро " << odd[i] << " -- " << odd[j]
					     << " (устранение нечётной степени)\n";
					added = true;
				}
			}
		}

		if (!added) {
			cout << "  Нет несмежных пар нечётных вершин — граф остаётся полуэйлеровым.\n";
			break;
		}
	}
}

/*
    LOOK: findCycle()
    Строит эйлеров цикл или путь алгоритмом Хирхольцера.
    Возвращает nullopt, если граф не является ни эйлеровым, ни полуэйлеровым.
*/
optional<vector<int>> EulerianCycle::findCycle() {
	if (!isEulerian() && !isSemiEulerian())
		return nullopt;

	// Начинаем с вершины нечётной степени (или любой ненулевой)
	int start = -1;
	auto odd = oddDegreeVertices();
	if (!odd.empty()) {
		start = odd[0];
	} else {
		for (int v = 0; v < n; v++) {
			if (degree(v) > 0) {
				start = v;
				break;
			}
		}
	}
	if (start == -1)
		return nullopt;

	return hierholzer(start);
}

/*
    LOOK: hierholzer(int start) const
    Алгоритм Хирхольцера: итеративный обход с удалением пройденных рёбер.
*/
optional<vector<int>> EulerianCycle::hierholzer(int start) const {
	auto adj = g.getAdjMatrix(); // рабочая копия — рёбра удаляем по ходу

	int totalEdges = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (adj[i][j] != 0)
				totalEdges++;

	vector<int> ptr(n, 0); // указатель на следующий непройденный сосед
	stack<int> stk;
	vector<int> circuit;
	circuit.reserve(totalEdges + 1);
	stk.push(start);

	while (!stk.empty()) {
		int u = stk.top();
		bool found = false;
		while (ptr[u] < n) {
			int v = ptr[u]++;
			if (adj[u][v] != 0) {
				adj[u][v] = 0;
				adj[v][u] = 0;
				stk.push(v);
				found = true;
				break;
			}
		}
		if (!found) {
			circuit.push_back(u);
			stk.pop();
		}
	}

	reverse(circuit.begin(), circuit.end());

	if ((int)circuit.size() != totalEdges + 1)
		return nullopt;

	return circuit;
}

/*
    LOOK: getAddedEdges() const
    Возвращает рёбра, добавленные при makeEulerian.
*/
const vector<pair<int, int>> &EulerianCycle::getAddedEdges() const {
	return addedEdges;
}

/*
    LOOK: printCycle(const vector<int> &cycle)
    Выводит эйлеров цикл или путь на экран.
*/
void EulerianCycle::printCycle(const vector<int> &cycle) {
	if (cycle.size() < 2)
		return;
	bool isCycle = (cycle.front() == cycle.back());
	cout << "\n  Эйлеров " << (isCycle ? "цикл" : "путь")
	     << " (" << cycle.size() - 1 << " рёбер):\n  ";
	for (size_t i = 0; i < cycle.size(); i++) {
		if (i) cout << " -> ";
		cout << cycle[i];
	}
	cout << "\n";
}

/*
    LOOK: printAddedEdges(const vector<pair<int, int>> &added)
    Выводит список рёбер, добавленных при преобразовании графа.
*/
void EulerianCycle::printAddedEdges(const vector<pair<int, int>> &added) {
	if (added.empty()) {
		cout << "  Граф уже был эйлеровым — рёбра не добавлялись.\n";
		return;
	}
	cout << "  Добавлено рёбер: " << added.size() << "\n";
	for (auto &[u, v] : added)
		cout << "    " << u << " -- " << v << "\n";
}
