#include "ui.h"
#include "../../common/utils/utils.h"
#include <iostream>
#include <limits>

using namespace std;

vector<int> Lab2UI::showFulkerson(Graph<int> &graph) {
	Fulkerson<int> fulkerson(graph);
	vector<int> order = fulkerson.computeOrder();
	fulkerson.printResult(order);
	return order;
}

void Lab2UI::showWarshall(Graph<int> &graph) {
	int n = graph.getNumVertices();

	FloydWarshall<int> fw(graph);
	fw.compute();

	cout << "\n== Алгоритм Флойда-Уоршалла ==\n";
	fw.printDistMatrix();

	int choice;
	cout << "\n  Посмотреть конкретный путь?\n";
	cout << "    1. Да\n";
	cout << "    2. Нет\n";
	cout << "  Ваш выбор [1/2]: ";
	if (!(cin >> choice) || choice != 1) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return;
	}

	int from, to;
	while (true) {
		cout << "  Начальная вершина [0-" << n - 1 << "]: ";
		if (cin >> from && from >= 0 && from < n)
			break;
		cout << "  Ошибка: введите номер от 0 до " << n - 1 << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while (true) {
		cout << "  Конечная вершина  [0-" << n - 1 << "]: ";
		if (cin >> to && to >= 0 && to < n)
			break;
		cout << "  Ошибка: введите номер от 0 до " << n - 1 << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	fw.printPath(from, to);
}

void Lab2UI::showComparison(Graph<int> &graph) {
	cout << "\n== Сравнение алгоритмов по числу итераций ==\n";

	Fulkerson<int> fulkerson(graph);
	fulkerson.computeOrder();
	int falkerIter = fulkerson.getIterCount();

	FloydWarshall<int> fw(graph);
	fw.compute();
	int warshallIter = fw.getIterCount();

	int n = graph.getNumVertices();
	cout << "  Вершин в графе: " << n << "\n\n";
	cout << "  Алгоритм Фалкерсона:      " << falkerIter << " итераций\n";
	cout << "  Алгоритм Флойда-Уоршалла: " << warshallIter
	     << " итераций  (= n^3 = " << n << "^3 = " << n * n * n << ")\n";
}

void Lab2UI::processChoice(int menuChoice, Graph<int> *graph) {
	if (!graph) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}

	if (menuChoice == 1) {
		vector<int> order = showFulkerson(*graph);
		if (!order.empty()) {
			Graph<int> reordered = graph->reorder(order);
			*graph = reordered;
			cout << "\n  Граф переупорядочен. Матрицы теперь отражают новую нумерацию.\n";
		}
	} else if (menuChoice == 2) {
		showWarshall(*graph);
	} else if (menuChoice == 3) {
		showComparison(*graph);
	}
}
