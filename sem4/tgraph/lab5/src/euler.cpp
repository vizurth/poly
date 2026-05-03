#include "euler.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

EulerCycle::EulerCycle(const Graph<int> &g) : graph(g.getNumVertices()) {
	int n = g.getNumVertices();
	auto adj = g.getAdjMatrix();
	auto wgt = g.getWeightMatrix();

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] == 0 && adj[j][i] == 0)
				continue;

			int w = adj[i][j] ? wgt[i][j] : wgt[j][i];
			if (w == 0)
				w = 1;
			addEdge(i, j, w);
		}
	}
}

int EulerCycle::degree(int v) const {
	auto adj = graph.getAdjMatrix();
	int d = 0;
	int n = graph.getNumVertices();
	for (int to = 0; to < n; to++) {
		if (to != v && adj[v][to] != 0)
			d++;
	}
	return d;
}

vector<int> EulerCycle::oddVertices() const {
	vector<int> odd;
	int n = graph.getNumVertices();
	for (int v = 0; v < n; v++) {
		if (degree(v) % 2 != 0)
			odd.push_back(v);
	}
	return odd;
}

vector<vector<int>> EulerCycle::connectedComponents() const {
	int n = graph.getNumVertices();
	auto adj = graph.getAdjMatrix();
	vector<bool> visited(n, false);
	vector<vector<int>> components;

	for (int start = 0; start < n; start++) {
		if (visited[start] || degree(start) == 0)
			continue;

		queue<int> q;
		vector<int> component;
		q.push(start);
		visited[start] = true;

		while (!q.empty()) {
			int v = q.front();
			q.pop();
			component.push_back(v);

			for (int to = 0; to < n; to++) {
				if (adj[v][to] != 0 && !visited[to]) {
					visited[to] = true;
					q.push(to);
				}
			}
		}

		components.push_back(component);
	}

	return components;
}

bool EulerCycle::isEulerian() const {
	return connectedComponents().size() <= 1 && oddVertices().empty();
}

bool EulerCycle::hasEdge(int u, int v) const {
	return graph.hasEdge(u, v);
}

void EulerCycle::addEdge(int u, int v, int w) {
	graph.addEdge(u, v, w);
	graph.addEdge(v, u, w);
}

void EulerCycle::removeEdge(int u, int v) {
	int n = graph.getNumVertices();
	auto adj = graph.getAdjMatrix();
	auto wgt = graph.getWeightMatrix();

	adj[u][v] = adj[v][u] = 0;
	wgt[u][v] = wgt[v][u] = 0;

	Graph<int> rebuilt(n);
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j] == 0 && adj[j][i] == 0)
				continue;
			int w = adj[i][j] ? wgt[i][j] : wgt[j][i];
			if (w == 0)
				w = 1;
			rebuilt.addEdge(i, j, w);
			rebuilt.addEdge(j, i, w);
		}
	}

	graph = rebuilt;
}

void EulerCycle::connectComponents(vector<EulerChange> &changes) {
	auto components = connectedComponents();
	if (components.size() <= 1)
		return;

	for (int i = 1; i < (int)components.size(); i++) {
		int u = components[i - 1][0];
		int v = components[i][0];
		addEdge(u, v, 1);
		changes.push_back(
		    {u, v, true, "добавлено ребро для соединения компонент"});
	}
}

void EulerCycle::makeEvenDegrees(vector<EulerChange> &changes) {
	vector<int> odd = oddVertices();
	int n = graph.getNumVertices();

	for (int i = 0; i + 1 < (int)odd.size(); i += 2) {
		int u = odd[i];
		int v = odd[i + 1];

		if (!hasEdge(u, v)) {
			addEdge(u, v, 1);
			changes.push_back({u, v, true, "добавлено ребро для чётных степеней"});
			continue;
		}

		int helper = -1;
		for (int w = 0; w < n; w++) {
			if (w == u || w == v)
				continue;
			if (!hasEdge(u, w) && !hasEdge(v, w)) {
				helper = w;
				break;
			}
		}

		if (helper != -1) {
			addEdge(u, helper, 1);
			addEdge(v, helper, 1);
			changes.push_back(
			    {u, helper, true, "добавлено ребро через вспомогательную вершину"});
			changes.push_back(
			    {v, helper, true, "добавлено ребро через вспомогательную вершину"});
		} else {
			removeEdge(u, v);
			changes.push_back({u, v, false, "удалено ребро для чётных степеней"});
		}
	}
}

vector<int> EulerCycle::buildEulerCycle() const {
	auto work = graph.getAdjMatrix();
	vector<int> cycle;
	int n = graph.getNumVertices();

	int start = 0;
	for (int i = 0; i < n; i++) {
		int d = 0;
		for (int to = 0; to < n; to++) {
			if (work[i][to] != 0)
				d++;
		}
		if (d > 0) {
			start = i;
			break;
		}
	}

	stack<int> st;
	st.push(start);

	while (!st.empty()) {
		int v = st.top();
		int to = -1;

		for (int i = 0; i < n; i++) {
			if (work[v][i] != 0) {
				to = i;
				break;
			}
		}

		if (to == -1) {
			cycle.push_back(v);
			st.pop();
		} else {
			work[v][to] = 0;
			work[to][v] = 0;
			st.push(to);
		}
	}

	reverse(cycle.begin(), cycle.end());
	return cycle;
}

EulerResult EulerCycle::process() {
	EulerResult result;
	result.initiallyEulerian = isEulerian();

	if (!result.initiallyEulerian) {
		connectComponents(result.changes);
		makeEvenDegrees(result.changes);
	}

	result.finalEulerian = isEulerian();
	if (result.finalEulerian)
		result.cycle = buildEulerCycle();

	return result;
}

Graph<int> EulerCycle::getGraph() const {
	return graph;
}

void EulerCycle::printReport(const EulerResult &result) {
	cout << "\n== Эйлеров цикл ==\n";
	cout << "  Граф является эйлеровым: "
	     << (result.initiallyEulerian ? "да" : "нет") << "\n";

	cout << "\n  Модификация:\n";
	if (result.initiallyEulerian) {
		cout << "    не требуется\n";
	} else if (result.changes.empty()) {
		cout << "    не выполнена\n";
	} else {
		for (auto &change : result.changes) {
			cout << "    " << (change.added ? "+ " : "- ")
			     << change.u << " -- " << change.v
			     << " (" << change.reason << ")\n";
		}
	}

	if (!result.finalEulerian) {
		cout << "\n  Не удалось получить эйлеров граф.\n";
		return;
	}

	cout << "\n  Построение эйлерова цикла:\n";
	if (result.cycle.size() <= 1) {
		cout << "    Эйлеров цикл тривиальный (в графе нет рёбер).\n";
		return;
	}

	cout << "    ";
	for (int i = 0; i < (int)result.cycle.size(); i++) {
		if (i > 0)
			cout << " -> ";
		cout << result.cycle[i];
	}
	cout << "\n";
}
