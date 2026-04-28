#include "ui.h"
#include "../../common/utils/utils.h"
#include <iostream>
#include <limits>

using namespace std;

void Lab1UI::showEccentricities(Graph<int> &graph) {
	int n = graph.getNumVertices();
	cout << "\n===================== Эксцентриситеты вершин =====================\n";
	vector<int> eccs = graph.allEccentricities();
	for (int i = 0; i < n; i++) {
		cout << "  Вершина " << i << ": ";
		if (eccs[i] == INF<int>)
			cout << "недостижима\n";
		else
			cout << eccs[i] << "\n";
	}
}

void Lab1UI::showCenter(Graph<int> &graph) {
	vector<int> centers = graph.findCenter();
	cout << "\n===================== Центр графа =====================\n";
	if (centers.empty()) {
		cout << "  Центр не найден (граф несвязный).\n";
	} else {
		cout << "  Вершины центра: ";
		for (int i = 0; i < (int)centers.size(); i++) {
			if (i > 0)
				cout << ", ";
			cout << centers[i];
		}
		cout << "\n";
		cout << "  Радиус графа: " << graph.eccentricity(centers[0]) << "\n";
	}
}

void Lab1UI::showDiameter(Graph<int> &graph) {
	vector<int> diametral = graph.findDiametral();
	cout << "\n===================== Диаметр графа =====================\n";
	if (diametral.empty()) {
		cout << "  Диаметр не определён (граф несвязный).\n";
	} else {
		int diameter = graph.eccentricity(diametral[0]);
		cout << "  Диаметр: " << diameter << "\n";
		cout << "  Периферийные вершины: ";
		for (int i = 0; i < (int)diametral.size(); i++) {
			if (i > 0)
				cout << ", ";
			cout << diametral[i];
		}
		cout << "\n\n  Периферийные пути:\n";

		for (int u : diametral) {
			vector<int> dists = graph.bfs(u);
			for (int v = 0; v < graph.getNumVertices(); v++) {
				if (dists[v] == diameter) {
					vector<int> path;
					int curr = v;
					while (curr != u) {
						path.push_back(curr);
						for (int prev = 0; prev < graph.getNumVertices(); prev++) {
							if (graph.hasEdge(prev, curr) && dists[prev] == dists[curr] - 1) {
								curr = prev;
								break;
							}
						}
					}
					path.push_back(u);
					cout << "    ";
					for (int k = (int)path.size() - 1; k >= 0; k--) {
						cout << path[k];
						if (k > 0)
							cout << " -> ";
					}
					cout << "\n";
				}
			}
		}
	}
}

void Lab1UI::showShimbell(Graph<int> &graph) {
	int choice;
	cout << "\n  Тип матрицы Шимбела:\n";
	cout << "    1. Минимальные пути\n";
	cout << "    2. Максимальные пути\n";
	while (true) {
		cout << "  Ваш выбор [1/2]: ";
		if (cin >> choice && (choice == 1 || choice == 2))
			break;
		cout << "  Ошибка: введите 1 или 2.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	int k;
	while (true) {
		cout << "  Длина пути K (от 0 до " << graph.getNumVertices() - 1 << "): ";
		if (cin >> k && k >= 0 && k < graph.getNumVertices())
			break;
		cout << "  Ошибка: введите целое число от 0 до " << graph.getNumVertices() - 1 << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	ShimbelSolver<int> solver(graph);
	bool findMin = (choice == 1);
	auto matrix  = findMin ? solver.computeShortestPaths(k) : solver.computeLongestPaths(k);
	string title = findMin
	    ? "Матрица Шимбела (минимальные пути, K=" + to_string(k) + ")"
	    : "Матрица Шимбела (максимальные пути, K=" + to_string(k) + ")";
	ShimbelSolver<int>::printShimbelMatrix(matrix, title);
}

void Lab1UI::showAllPaths(Graph<int> &graph) {
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
	cout << "\n===================== Все пути из " << from << " в " << to
	     << " =====================\n";
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

void Lab1UI::processChoice(int menuChoice, Graph<int> *graph) {
	if (!graph) {
		cout << "  Сначала сгенерируйте граф (пункт 1).\n";
		return;
	}

	if (menuChoice == 1)
		showEccentricities(*graph);
	else if (menuChoice == 2)
		showCenter(*graph);
	else if (menuChoice == 3)
		showDiameter(*graph);
	else if (menuChoice == 4)
		showShimbell(*graph);
	else if (menuChoice == 5)
		showAllPaths(*graph);
}
