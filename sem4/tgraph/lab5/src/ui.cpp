#include "ui.h"
#include <iostream>
#include <limits>

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

vector<int> Lab5UI::askCycleIndices(int totalCycles) {
	vector<int> indices;
	vector<bool> used(totalCycles + 1, false);

	cout << "\n  Введите номера фундаментальных циклов для симметрической разности.\n";
	cout << "  Завершение ввода: 0\n";

	while (true) {
		int idx;
		cout << "  Номер цикла [1-" << totalCycles << "] или 0: ";
		if (!(cin >> idx)) {
			cout << "  Ошибка: введите целое число.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (idx == 0)
			break;
		if (idx < 1 || idx > totalCycles) {
			cout << "  Ошибка: номер вне диапазона.\n";
			continue;
		}
		if (!used[idx]) {
			used[idx] = true;
			indices.push_back(idx);
		}
	}

	return indices;
}

void Lab5UI::showCycleBasis(Graph<int> *graph) {
	CycleBasis solver(*graph);
	if (!solver.build()) {
		cout << "  Не удалось построить остов. Граф может быть несвязным.\n";
		return;
	}

	cout << "\n== Циклы через остов и симметрическую разность ==\n";
	Kruskal::printMST(solver.getMSTEdges());
	CycleBasis::printFundamentalCycles(solver.getFundamentalCycles());

	const auto &basis = solver.getFundamentalCycles();
	if (basis.empty())
		return;

	cout << "\n  Построить цикл(ы) симметрической разностью?\n";
	cout << "    1. Да\n";
	cout << "    2. Нет\n";
	cout << "  Ваш выбор [1/2]: ";

	int ch = 2;
	if (!(cin >> ch)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return;
	}
	if (ch != 1)
		return;

	vector<int> indices = askCycleIndices((int)basis.size());
	if (indices.empty()) {
		cout << "  Циклы не выбраны.\n";
		return;
	}

	SymDiffResult result = solver.symmetricDifference(indices);
	CycleBasis::printSymDiffResult(result);
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
	else if (menuChoice == 2)
		showCycleBasis(graph);
}
