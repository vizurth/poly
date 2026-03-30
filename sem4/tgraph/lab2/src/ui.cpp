#include "ui.h"
#include "../../common/utils/utils.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

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

void UI::showGraph(Graph<int> &graph) {
	cout << "\n";
	graph.printAdjMatrix();
	cout << "\n";
	graph.printWeightMatrix();
	cout << "\n";
	graph.printEdges();
}

void UI::showAllPaths(Graph<int> &graph) {
	int n = graph.getNumVertices();
	int from, to;

	while (true) {
		cout << "  Начальная вершина A [0-" << n - 1 << "]: ";
		if (cin >> from && from >= 0 && from < n)
			break;
		cout << "  Ошибка: введите номер от 0 до " << n - 1 << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while (true) {
		cout << "  Конечная вершина B [0-" << n - 1 << "]: ";
		if (cin >> to && to >= 0 && to < n)
			break;
		cout << "  Ошибка: введите номер от 0 до " << n - 1 << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	auto paths = graph.findAllPaths(from, to);

	cout << "\n== Все пути из " << from << " в " << to << " ==\n";
	if (paths.empty()) {
		cout << "  Путей нет.\n";
		return;
	}

	cout << "  Найдено путей: " << paths.size() << "\n\n";
	for (int i = 0; i < (int)paths.size(); i++) {
		cout << "  " << i + 1 << ". ";
		for (int j = 0; j < (int)paths[i].size(); j++) {
			if (j > 0)
				cout << " -> ";
			cout << paths[i][j];
		}
		cout << "\n";
	}
}

vector<int> UI::showFulkerson(Graph<int> &graph) {
	Fulkerson<int> fulkerson(graph);

	vector<int> order = fulkerson.computeOrder();
	fulkerson.printResult(order);
	return order;
}

void UI::showWarshall(Graph<int> &graph) {
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

void UI::showComparison(Graph<int> &graph) {
	cout << "\n== Сравнение алгоритмов по числу итераций ==\n";

	// Фалкерсон
	Fulkerson<int> fulkerson(graph);
	fulkerson.computeOrder();
	int falkerIter = fulkerson.getIterCount();

	// Флойд-Уоршалл
	FloydWarshall<int> fw(graph);
	fw.compute();
	int warshallIter = fw.getIterCount();

	int n = graph.getNumVertices();
	cout << "  Вершин в графе: " << n << "\n\n";
	cout << "  Алгоритм Фалкерсона:    " << falkerIter << " итераций\n";
	cout << "  Алгоритм Флойда-Уоршалла: " << warshallIter
	     << " итераций  (= n^3 = " << n << "^3 = " << n * n * n << ")\n";
}

void UI::run() {
	cout << "== Генератор случайных графов ==\n";

	Graph<int> *currentGraph = nullptr;

	int menuChoice;
	do {
		cout << "\n  Главное меню:\n";
		cout << "    1. Сгенерировать граф\n";
		cout << "    2. Вывести матрицу смежности\n";
		cout << "    3. Вывести матрицу весов\n";
		cout << "    4. Топологическая сортировка (алгоритм Фалкерсона)\n";
		cout << "    5. Кратчайшие пути (алгоритм Флойда-Уоршалла)\n";
		cout << "    6. Сравнить алгоритмы по числу итераций\n";
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
			cout << "\n== Параметры графа ==\n";
			int n = askNumVertices();
			bool directed = askDirected();
			WeightType wt = askWeightType();

			try {
				delete currentGraph;
				currentGraph = new Graph<int>(
				    Generator<int>(n, directed, wt, 10.0, 2.0, 10.0, 2.0)
				        .generateGraph());

				cout << "\nГраф сгенерирован\n";
			} catch (const exception &e) {
				cout << "  Ошибка генерации: " << e.what() << "\n";
			}
		}

		if (menuChoice == 2) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				currentGraph->printAdjMatrix();
			}
		}

		if (menuChoice == 3) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				currentGraph->printWeightMatrix();
				currentGraph->printEdges();
			}
		}

		if (menuChoice == 4) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				vector<int> order = showFulkerson(*currentGraph);
				if (!order.empty()) {
					Graph<int> *reordered = new Graph<int>(currentGraph->reorder(order));
					delete currentGraph;
					currentGraph = reordered;
					cout << "\n  Граф переупорядочен. Матрицы теперь отражают новую нумерацию.\n";
				}
			}
		}

		if (menuChoice == 5) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showWarshall(*currentGraph);
			}
		}

		if (menuChoice == 6) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showComparison(*currentGraph);
			}
		}

	} while (menuChoice != 0);

	delete currentGraph;
	cout << "\nДо свидания!\n";
}
