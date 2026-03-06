#include "ui.h"
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// ──────────────────────────────────────────────
//  Вспомогательные приватные методы
// ──────────────────────────────────────────────

int UI::askNumVertices() {
	int n;
	while (true) {
		cout << "  Введите количество вершин (>= 2): ";
		if (cin >> n && n >= 2)
			return n;
		cout << "  Ошибка: введите целое число >= 2.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

bool UI::askDirected() {
	int choice;
	cout << "\n  Тип графа:\n";
	cout << "    1. Ориентированный\n";
	cout << "    2. Неориентированный\n";
	while (true) {
		cout << "  Ваш выбор [1/2]: ";
		if (cin >> choice && (choice == 1 || choice == 2))
			return choice == 1;
		cout << "  Ошибка: введите 1 или 2.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

WeightType UI::askWeightType() {
	int choice;
	cout << "\n  Тип весов рёбер:\n";
	cout << "    1. Только положительные\n";
	cout << "    2. Только отрицательные\n";
	cout << "    3. Смешанные (положительные и отрицательные)\n";
	while (true) {
		cout << "  Ваш выбор [1-3]: ";
		if (cin >> choice && choice >= 1 && choice <= 3) {
			if (choice == 1)
				return WeightType::POSITIVE;
			if (choice == 2)
				return WeightType::NEGATIVE;
			return WeightType::MIXED;
		}
		cout << "  Ошибка: введите число от 1 до 3.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void UI::showGraph(Graph<double> &graph) {
	cout << "\n";
	graph.printAdjMatrix();
	cout << "\n";
	graph.printEdges();
}

// ──────────────────────────────────────────────
//  Главное меню
// ──────────────────────────────────────────────

void UI::run() {
	cout << "╔══════════════════════════════════════╗\n";
	cout << "║     Генератор случайных графов       ║\n";
	cout << "╚══════════════════════════════════════╝\n";

	int menuChoice;
	do {
		cout << "\n  Главное меню:\n";
		cout << "    1. Сгенерировать граф\n";
		cout << "    0. Выход\n";
		cout << "  Ваш выбор: ";

		if (!(cin >> menuChoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (menuChoice == 0)
			break;

		if (menuChoice == 1) {
			cout << "\n── Параметры графа ─────────────────────\n";
			int n = askNumVertices();
			bool directed = askDirected();
			WeightType wt = askWeightType();

			try {
				Generator<double> gen(n, directed, wt, 10.0, 2.0, 10.0, 2.0);
				Graph<double> graph = gen.generateGraph();

				cout << "\n── Результат ───────────────────────────\n";
				showGraph(graph);
			} catch (const exception &e) {
				cout << "  Ошибка генерации: " << e.what() << "\n";
			}
		}

	} while (menuChoice != 0);

	cout << "\nДо свидания!\n";
}
