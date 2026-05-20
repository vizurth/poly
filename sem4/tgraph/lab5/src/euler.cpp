#include "euler.h"
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

/*
    LOOK: EulerianCycle(const Graph<int> &graph)
    Конструктор принимает граф и сохраняет его копию.
*/
EulerianCycle::EulerianCycle(const Graph<int> &graph)
    : g(graph), n(graph.getNumVertices()) {}

/*
    LOOK: degree(int v)
    Возвращает степень вершины v.
*/
int EulerianCycle::degree(int v) const {
	auto adj = g.getAdjMatrix();
	int d = 0;
	for (int j = 0; j < n; j++)
		if (adj[v][j] != 0) d++;
	
	return d;
}

/*
    LOOK: oddVertices()
    Возвращает список вершин с нечётной степенью.
*/
vector<int> EulerianCycle::oddVertices() const {
	vector<int> odd;
	for (int v = 0; v < n; v++) {
		if (degree(v) % 2 != 0) {
			odd.push_back(v);
		}
	}
	return odd;
}

/*
    LOOK: isBridge(int u, int v)
    Проверяет, является ли ребро (u,v) мостом. Удаляем ребро и смотрим через BFS, достижима ли v из u. Если нет, ребро - мост.
*/
bool EulerianCycle::isBridge(int u, int v) const {
	auto adj = g.getAdjMatrix();
	adj[u][v] = adj[v][u] = 0; // удаляем ребро (u,v)

	vector<bool> vis(n, false);
	queue<int> q;
	q.push(u);
	vis[u] = true;
	while (!q.empty()) {
		int curr = q.front();
		q.pop();
		for (int w = 0; w < n; w++) {
			if (!vis[w] && adj[curr][w]) {
				vis[w] = true;
				q.push(w);
			}
		}
	}
	return !vis[v]; // v недостижима из u
}

/*
    LOOK: isEulerian()
    Проверяет, является ли граф эйлеровым. Граф эйлеров, если он связный и все вершины имеют чётную степень. Связность гарантируется при генерации графа.
*/
bool EulerianCycle::isEulerian() const { return oddVertices().empty(); }

/*
    LOOK: makeEulerian()
    Пока есть нечётные вершины, выбираем пару и пытаемся исправить:
    - если нет ребра, добавляем его.
    - если есть ребро, но оно не мост, удаляем его.
    - если есть ребро и оно - мост, пропускаем эту пару и идем дальше.
    Если не удалось исправить ни одну пару, граф остаётся полуэйлеровым.
*/
void EulerianCycle::makeEulerian() {
	for (int iter = 1;; iter++) {
		auto odd = oddVertices();
		if (odd.empty())
			break;

		cout << "  [шаг " << iter << "] нечётных вершин: " << odd.size()
		     << " -> {";
		for (size_t i = 0; i < odd.size(); i++) {
			if (i) {
				cout << ", ";
			}
			cout << odd[i];
		}
		cout << "}\n";

		bool fixed = false;
		for (size_t i = 0; i < odd.size() && !fixed; i++) {
			for (size_t j = i + 1; j < odd.size() && !fixed; j++) {
				int u = odd[i], v = odd[j];

				if (!g.hasEdge(u, v)) {
					g.addEdge(u, v, 1);
					g.addEdge(v, u, 1);
					cout << "    [+] добавляем " << u << " -- " << v << "\n";
					fixed = true;
				} else if (!isBridge(u, v)) {
					g.removeEdge(u, v);
					g.removeEdge(v, u);
					cout << "    [-] удаляем  " << u << " -- " << v
					     << " (ребро есть, не мост)\n";
					fixed = true;
				} else {
					cout << "    [!] " << u << " -- " << v
					     << " - мост, пропускаем\n";
				}
			}
		}

		if (!fixed) {
			cout << "  Не удалось исправить ни одну пару.\n";
			break;
		}
	}

	auto odd = oddVertices();
	if (odd.empty()) {
		cout << "  Граф стал эйлеровым.\n";
	} else if (odd.size() == 2) {
		cout << "  Граф стал полуэйлеровым (ровно 2 вершины нечётной "
		        "степени).\n";
	} else {
		cout << "  Граф не эйлеров и не полуэйлеров (" << odd.size()
		     << " вершин нечётной степени).\n";
	}
}

/*
    LOOK: buildCycle()
    Строим эйлеров цикл.
    - берем стартовую вершину.
    - Потка есть рёбра, идём по ним, удаляя пройденные ребра. Если из текущей вершины нет рёбер, добавляем её в результат и откатываемся назад.
*/
vector<int> EulerianCycle::buildCycle() const {
	int start = -1;
	for (int v = 0; v < n; v++) {
		if (degree(v) > 0) {
			start = v;
			break;
		}
	}
	if (start == -1) {
		return {};
	}

	auto adj = g.getAdjMatrix();

	int totalEdges = g.getEdgeCount();

	stack<int> S;
	vector<int> result;
	S.push(start);

	while (!S.empty()) {
		int v = S.top();
		int u = -1;
		for (int j = 0; j < n; j++) {
			if (adj[v][j]) {
				u = j;
				break;
			}
		}

		if (u == -1) {
			S.pop();
			result.push_back(v);
		} else {
			S.push(u);
			adj[v][u] = adj[u][v] = 0;
		}
	}

	if ((int)result.size() != totalEdges + 1) {
		return {};
	}
	return result;
}

/*
    LOOK: getGraph() const
    Получаем граф, для которого строится эйлеров цикл.
*/
const Graph<int> &EulerianCycle::getGraph() const { return g; }

/*
    LOOK: printCycle(const vector<int> &cycle)
    Выводим эйлеров цикл на экран.
*/
void EulerianCycle::printCycle(const vector<int> &cycle) {
	if (cycle.size() < 2) {
		return;
	}
	bool isCycle = (cycle.front() == cycle.back());
	cout << "\n  Эйлеров " << (isCycle ? "цикл" : "путь") << " ("
	     << cycle.size() - 1 << " рёбер):\n  ";
	for (size_t i = 0; i < cycle.size(); i++) {
		if (i) {
			cout << " -> ";
		}
		cout << cycle[i];
	}
	cout << "\n";
}
