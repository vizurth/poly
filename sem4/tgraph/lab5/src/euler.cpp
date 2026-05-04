#include "euler.h"
#include "traversal.h"
#include <algorithm>
#include <iostream>
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

	initialAdj = graph.getAdjMatrix();
}

int EulerCycle::degree(int v) const {
	auto adj = graph.getAdjMatrix();
	return Lab5Traversal::degree(adj, v);
}

vector<int> EulerCycle::oddVertices() const {
	vector<int> odd;
	auto adj = graph.getAdjMatrix();
	int n = (int)adj.size();
	for (int v = 0; v < n; v++) {
		if (Lab5Traversal::degree(adj, v) % 2 != 0)
			odd.push_back(v);
	}
	return odd;
}

vector<vector<int>> EulerCycle::connectedComponents() const {
	return Lab5Traversal::components(graph.getAdjMatrix(), true);
}

bool EulerCycle::isEulerian() const {
	return connectedComponents().size() <= 1 && oddVertices().empty();
}

bool EulerCycle::hasEdge(int u, int v) const {
	auto adj = graph.getAdjMatrix();
	return adj[u][v] != 0 || adj[v][u] != 0;
}

void EulerCycle::addEdge(int u, int v, int w) {
	graph.addEdge(u, v, w);
	graph.addEdge(v, u, w);
}

bool EulerCycle::removeEdge(int u, int v) {
	if (!hasEdge(u, v))
		return false;

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
	return true;
}

bool EulerCycle::isInitialEdge(int u, int v) const {
	return initialAdj[u][v] != 0 || initialAdj[v][u] != 0;
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
			changes.push_back(
			    {u, v, true, "добавлено ребро для чётных степеней"});
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
			    {u, helper, true,
			     "добавлено ребро через вспомогательную вершину"});
			changes.push_back(
			    {v, helper, true,
			     "добавлено ребро через вспомогательную вершину"});
		} else if (removeEdge(u, v)) {
			changes.push_back(
			    {u, v, false,
			     isInitialEdge(u, v)
			         ? "удалено ребро из исходного графа для чётных степеней"
			         : "удалено ранее добавленное ребро для чётных степеней"});
		}
	}
}

vector<int> EulerCycle::buildEulerCycle() const {
	int n = graph.getNumVertices();
	auto work = graph.getAdjMatrix();
	vector<int> cycle;
	stack<int> st;

	int start = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (work[i][j])
				start = i;
	st.push(start);

	while (!st.empty()) {
		int v = st.top();
		int u = -1;

		for (int i = 0; i < n; i++) {
			if (work[v][i]) {
				u = i;
				break;
			}
		}

		if (u == -1) {
			cycle.push_back(v);
			st.pop();
		} else {
			st.push(u);
			work[v][u] = work[u][v] = 0;
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

Graph<int> EulerCycle::getGraph() const { return graph; }

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
			cout << "    " << (change.added ? "+ " : "- ") << change.u << " -- "
			     << change.v << " (" << change.reason << ")\n";
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
