#include "ui.h"
#include <iostream>

using namespace std;

void Lab5UI::showEulerCycle(Graph<int> *graph) {
	EulerCycle solver(*graph);
	EulerResult result = solver.process();

	EulerCycle::printReport(result);

	if (!result.changes.empty()) {
		*graph = solver.getGraph();
		cout << "\n  Граф обновлён в памяти с учётом модификаций.\n";
	}
}

void Lab5UI::onNewGraph(bool directed) {
	isDirected = directed;
}

void Lab5UI::processChoice(int menuChoice, Graph<int> *graph) {
	if (!graph) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}
	if (isDirected) {
		cout << "  Лабораторная работа №5 требует неориентированный граф.\n";
		cout << "  Сгенерируйте неориентированный граф (пункт 1).\n";
		return;
	}

	if (menuChoice == 1)
		showEulerCycle(graph);
}
