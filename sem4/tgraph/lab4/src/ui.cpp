#include "ui.h"
#include <iostream>
#include <limits>
using namespace std;

Graph<int> Lab4UI::buildMSTGraph(const vector<Edge> &edges, int n) {
	Graph<int> g(n);
	for (auto &e : edges) {
		g.addEdge(e.u, e.v, e.w);
		g.addEdge(e.v, e.u, e.w);
	}
	return g;
}

void Lab4UI::showKirchhoff(Graph<int> *g) {
	int n = g->getNumVertices();
	auto adj = g->getAdjMatrix();

	// строим Лапласиан для отображения
	vector<vector<int>> L(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j && (adj[i][j] != 0 || adj[j][i] != 0))
				L[i][j] = -1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (L[i][j] == -1)
				L[i][i]++;

	Kirchhoff::printLaplacian(L, n);
	long long cnt = Kirchhoff::count(*g);
	Kirchhoff::printResult(cnt);
}

void Lab4UI::showKruskal(Graph<int> *g) {
	Kruskal kr(*g);
	mstEdges = kr.compute();

	if (mstEdges.empty()) {
		cout << "  Граф несвязный — остов не существует.\n";
		hasMST = false;
		return;
	}

	hasMST = true;
	hasPrufer = false;
	Kruskal::printMST(mstEdges);
}

void Lab4UI::showPruferEncode() {
	if (!hasMST) {
		cout << "  Сначала постройте минимальный остов (пункт 17).\n";
		return;
	}
	pruferCode = Prufer::encode(mstEdges, (int)mstEdges.size() + 1);
	hasPrufer = true;
	Prufer::printEdges(mstEdges, "Исходный остов:");
	Prufer::printCode(pruferCode);
}

void Lab4UI::showPruferDecode() {
	if (!hasPrufer) {
		cout << "  Сначала закодируйте остов (пункт 18).\n";
		return;
	}
	vector<Edge> decoded = Prufer::decode(pruferCode);
	Prufer::printCode(pruferCode);
	Prufer::printEdges(decoded, "Декодированный остов:");
}

void Lab4UI::showMIS(Graph<int> *g) {
	cout << "\n  Выберите граф для MIS:\n";
	cout << "    1. Исходный граф\n";
	cout << "    2. Минимальный остов (MST)\n";

	if (!hasMST)
		cout << "    (MST ещё не построен)\n";

	int ch;
	cout << "  Ваш выбор [1/2]: ";
	if (!(cin >> ch)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return;
	}

	if (ch == 2 && !hasMST) {
		cout << "  Сначала постройте минимальный остов (пункт 17).\n";
		return;
	}

	if (ch == 2) {
		int n = (int)mstEdges.size() + 1;
		Graph<int> mstGraph = buildMSTGraph(mstEdges, n);
		cout << "\n  Запуск на минимальном остове...\n";
		MIS mis(mstGraph.getAdjMatrix());
		MIS::printResult(mis.compute());
	} else {
		cout << "\n  Запуск на исходном графе...\n";
		MIS mis(g->getAdjMatrix());
		MIS::printResult(mis.compute());
	}
}

void Lab4UI::onNewGraph(bool directed) {
	isDirected = directed;
	hasMST = false;
	hasPrufer = false;
}

void Lab4UI::processChoice(int menuChoice, Graph<int> *g) {
	if (!g) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}
	if (isDirected) {
		cout << "  Лабораторная работа №4 требует неориентированный граф.\n";
		cout << "  Сгенерируйте неориентированный граф (пункт 1).\n";
		return;
	}

	if (menuChoice == 1)
		showKirchhoff(g);
	else if (menuChoice == 2)
		showKruskal(g);
	else if (menuChoice == 3)
		showPruferEncode();
	else if (menuChoice == 4)
		showPruferDecode();
	else if (menuChoice == 5)
		showMIS(g);
}
