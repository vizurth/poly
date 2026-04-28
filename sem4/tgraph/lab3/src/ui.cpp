#include "ui.h"
#include "../../common/distribution/weibull.h"
#include "../../common/utils/utils.h"
#include <iostream>
#include <limits>
using namespace std;

pair<int, int> Lab3UI::askSourceSink(int n) {
	int s, t;
	while (true) {
		cout << "  Источник [0-" << n - 1 << "]: ";
		if (cin >> s && s >= 0 && s < n)
			break;
		cout << "  Ошибка.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while (true) {
		cout << "  Сток    [0-" << n - 1 << "]: ";
		if (cin >> t && t >= 0 && t < n && t != s)
			break;
		cout << "  Ошибка (должна отличаться от источника).\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return {s, t};
}

vector<vector<int>> Lab3UI::generateCostMatrix(const Graph<int> &graph) {
	int n = graph.getNumVertices();
	auto adj = graph.getAdjMatrix();
	WeibullDistribution costDist(10.0, 2.0);
	vector<vector<int>> cost(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (adj[i][j] != 0)
				cost[i][j] = max(1, (int)costDist.generate());
	return cost;
}

void Lab3UI::showCost(const vector<vector<int>> &cost, int n) {
	printMatrix(cost, "Матрица стоимости:");
	cout << "\nСтоимости рёбер:\n";
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (cost[i][j] != 0)
				cout << "  " << i << " -> " << j
				     << "  (стоимость: " << cost[i][j] << ")\n";
}

int Lab3UI::showMaxFlow(const Graph<int> &graph, int s, int t) {
	MaxFlow<int> mf(graph);
	int flow = mf.compute(s, t);
	cout << "\n== Алгоритм Форда-Фалкерсона (Edmonds-Karp) ==\n";
	mf.printResult(flow, s, t);
	return flow;
}

void Lab3UI::showMinCostFlow(const Graph<int> &graph,
                             const vector<vector<int>> &cost, int s, int t,
                             int targetFlow) {
	MinCostFlow<int> mcf(graph, cost);
	auto [flow, totalCost] = mcf.compute(s, t, targetFlow);
	mcf.printResult(flow, totalCost, targetFlow, s, t);
}

void Lab3UI::onNewGraph(const Graph<int> &graph, int n) {
	costMatrix = generateCostMatrix(graph);
	source = 0;
	sink = n - 1;
	hasMaxFlow = false;
	lastMaxFlow = 0;
}

void Lab3UI::processChoice(int menuChoice, Graph<int> *graph) {
	if (!graph) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}

	if (menuChoice == 1) {
		printMatrix(graph->getWeightMatrix(),
		            "Матрица пропускных способностей:");
	} else if (menuChoice == 2) {
		showCost(costMatrix, graph->getNumVertices());
	} else if (menuChoice == 3) {
		cout << "\n  Текущий источник: " << source << ",  сток: " << sink
		     << "\n";
		cout << "  Изменить? [1-да / 2-нет]: ";
		int ch;
		if (cin >> ch && ch == 1)
			tie(source, sink) = askSourceSink(graph->getNumVertices());
		lastMaxFlow = showMaxFlow(*graph, source, sink);
		hasMaxFlow = true;
	} else if (menuChoice == 4) {
		if (!hasMaxFlow) {
			cout << "  Сначала найдите максимальный поток (пункт 14).\n";
		} else {
			int target = (lastMaxFlow * 2) / 3;
			cout << "\n== Поток минимальной стоимости (Флойд-Уоршалл) ==\n";
			cout << "\n  Максимальный поток: " << lastMaxFlow
			     << ",  целевой [2/3]: " << target << "\n";
			showMinCostFlow(*graph, costMatrix, source, sink, target);
		}
	}
}
