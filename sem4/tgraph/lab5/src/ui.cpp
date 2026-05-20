#include "ui.h"
#include "lab4/src/kruskal.h"
#include <iostream>
#include <string>
using namespace std;

/*
    LOOK: showEuler(Graph<int> *g)
    Проверяет эйлеровость графа, при необходимости модифицирует его
    и строит эйлеров цикл/путь алгоритмом Хирхольцера.
*/
void Lab5UI::showEuler(Graph<int> *g) {
	EulerianCycle euler(*g);

	if (euler.isEulerian()) {
		cout << "\n  Граф является эйлеровым.\n";
	} else {
		cout << "\n  Граф не является эйлеровым. Модификация:\n";
		euler.makeEulerian();
		*g = euler.getGraph(); // обновляем оригинальный граф
	}

	auto cycle = euler.buildCycle();
	if (!cycle.empty()) {
		EulerianCycle::printCycle(cycle);
	} else {
		cout << "\n  Не удалось построить эйлеров цикл.\n";
	}
}

/*
    LOOK: showCycleBasis(Graph<int> *g)
    Строит MST алгоритмом Краскала и выводит фундаментальную систему циклов.
*/
void Lab5UI::showCycleBasis(Graph<int> *g) {
	Kruskal kr(*g);
	mstEdges = kr.compute();

	hasMST = true;
	Kruskal::printMST(mstEdges);

	cycleBasis.compute(mstEdges, *g);
	cycleBasis.printBasis();
}

/*
    LOOK: showSymDiff()
    Читает номера циклов от пользователя и выводит их симметрическую разность.
*/
void Lab5UI::showSymDiff() {
	if (!hasMST) {
		cout << "  Сначала постройте MST и систему циклов (пункт 22).\n";
		return;
	}

	const auto &basis = cycleBasis.getBasis();
	if (basis.empty()) {
		cout << "  Фундаментальная система циклов пуста.\n";
		return;
	}

	cycleBasis.printBasis();
	cout << "\n  Введите номера циклов через пробел:\n  > ";

	string line;
	cin.ignore();
	getline(cin, line);

	vector<int> idx;
	for (size_t i = 0, j; i < line.size(); i = j) {
		while (i < line.size() && line[i] == ' ') {
			i++;
		}

		if (i >= line.size()) {
			break;
		}

		j = i;

		while (j < line.size() && line[j] != ' ') {
			j++;
		}

		int v = stoi(line.substr(i, j - i)) - 1;

		if (v >= 0 && v < (int)basis.size()) {
			idx.push_back(v);
		} else {
			cout << "  Индекс " << v + 1 << " вне диапазона. \n";
		}
	}

	if ((int)idx.size() < 2) {
		cout << "  Нужно выбрать хотя бы 2 цикла.\n";
		return;
	}

	EdgeSet result = basis[idx[0]];
	cout << "\n  Операция: C" << idx[0] + 1;
	for (size_t k = 1; k < idx.size(); k++) {
		result = CycleBasis::symDiff(result, basis[idx[k]]);
		cout << " △ C" << idx[k] + 1;
	}

	cout << "\n\n  Результат симметрической разности:\n";
	if (result.empty()) {
		cout << "    (пустое множество)\n";
	} else {
		for (auto &[u, v] : result) {
			cout << "    " << u << " -- " << v << "\n";
		}
	}
}

void Lab5UI::onNewGraph(bool directed) {
	isDirected = directed;
	hasMST = false;
}

void Lab5UI::processChoice(int choice, Graph<int> *g) {
	if (!g) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}
	if (isDirected) {
		cout << "  Лабораторная работа №5 требует неориентированный граф.\n";
		cout << "  Сгенерируйте неориентированный граф (пункт 1).\n";
		return;
	}

	if (choice == 1) {
		showEuler(g);
	} else if (choice == 2) {
		showCycleBasis(g);
	} else if (choice == 3) {
		showSymDiff();
	}
}
