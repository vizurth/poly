#include "ui.h"
#include "lab4/src/kruskal.h"
#include <iostream>
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
	} else if (euler.isSemiEulerian()) {
		cout << "\n  Граф является полуэйлеровым (найдём эйлеров путь).\n";
	} else {
		cout << "\n  Граф не является эйлеровым. Модификация:\n";
		euler.makeEulerian();
		EulerianCycle::printAddedEdges(euler.getAddedEdges());
	}

	auto cycle = euler.findCycle();
	if (cycle)
		EulerianCycle::printCycle(*cycle);
	else
		cout << "\n  Не удалось построить эйлеров цикл.\n";
}

/*
    LOOK: showCycleBasis(Graph<int> *g)
    Строит MST алгоритмом Краскала и выводит фундаментальную систему циклов.
*/
void Lab5UI::showCycleBasis(Graph<int> *g) {
	Kruskal kr(*g);
	mstEdges = kr.compute();

	if (mstEdges.empty()) {
		cout << "  Граф несвязный — MST не существует.\n";
		hasMST = false;
		return;
	}

	hasMST = true;
	Kruskal::printMST(mstEdges);

	cycleBasis.compute(mstEdges, *g);
	cycleBasis.printBasis();
}

/*
    LOOK: showSymDiff()
    Запускает интерактивный расчёт симметрической разности циклов.
*/
void Lab5UI::showSymDiff() {
	if (!hasMST) {
		cout << "  Сначала постройте MST и систему циклов (пункт 22).\n";
		return;
	}
	cycleBasis.interactiveSymDiff();
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

	if (choice == 1)
		showEuler(g);
	else if (choice == 2)
		showCycleBasis(g);
	else if (choice == 3)
		showSymDiff();
}
