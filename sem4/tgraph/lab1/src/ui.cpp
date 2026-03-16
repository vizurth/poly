#include "ui.h"
#include "../../common/utils/utils.h"
#include <iomanip>
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

void UI::showEccentricities(Graph<double> &graph) {
	int n = graph.getNumVertices();

	cout << "\n===================== Эксцентриситеты вершин "
	        "=====================\n";
	vector<int> eccs = graph.allEccentricities();
	for (int i = 0; i < n; i++) {
		cout << "  Вершина " << i << ": ";
		if (eccs[i] == INF<int>) {
			cout << "недостижима\n";
		} else {
			cout << eccs[i] << "\n";
		}
	}
}

void UI::showCenter(Graph<double> &graph) {
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

void UI::showDiameter(Graph<double> &graph) {
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
						if (k > 0) cout << " -> ";
					}
					cout << "\n";
				}
			}
		}
	}
}

void UI::showShimbell(Graph<double> &graph) {
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
		cout << "  Ошибка: введите целое число от 0 до " << graph.getNumVertices() - 1 << '.' << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	ShimbelSolver<double> solver(graph);
	bool findMin = (choice == 1);
	auto matrix = findMin ? solver.computeShortestPaths(k)
	                      : solver.computeLongestPaths(k);

	string title =
	    findMin ? "Матрица Шимбела (минимальные пути, K=" + to_string(k) + ")"
	            : "Матрица Шимбела (максимальные пути, K=" + to_string(k) + ")";
	ShimbelSolver<double>::printShimbelMatrix(matrix, title);
}

void UI::showAllPaths(Graph<double> &graph) {
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

// ──────────────────────────────────────────────
//  Главное меню
// ──────────────────────────────────────────────

void UI::run() {
	cout << "===================== Генератор случайных графов "
	        "=====================\n";

	Graph<double> *currentGraph = nullptr;

	int menuChoice;
	do {
		cout << "\n  Главное меню:\n";
		cout << "    1. Сгенерировать граф\n";
		cout << "    2. Вывести матрицу смежности\n";
		cout << "    3. Вывести матрицу весов\n";
		cout << "    4. Вычислить эксцентриситеты вершин\n";
		cout << "    5. Найти центр графа\n";
		cout << "    6. Найти диаметр графа\n";
		cout << "    7. Матрица Шимбела\n";
		cout << "    8. Найти все пути от A до B\n";
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
			cout << "\n===================== Параметры графа "
			        "=====================\n";
			int n = askNumVertices();
			bool directed = askDirected();
			WeightType wt = askWeightType();

			try {
				delete currentGraph;
				currentGraph = new Graph<double>(
				    Generator<double>(n, directed, wt, 10.0, 2.0, 10.0, 2.0)
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
				showEccentricities(*currentGraph);
			}
		}

		if (menuChoice == 5) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showCenter(*currentGraph);
			}
		}

		if (menuChoice == 6) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showDiameter(*currentGraph);
			}
		}

		if (menuChoice == 7) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showShimbell(*currentGraph);
			}
		}

		if (menuChoice == 8) {
			if (!currentGraph) {
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			} else {
				showAllPaths(*currentGraph);
			}
		}

	} while (menuChoice != 0);

	delete currentGraph;
	cout << "\nДо свидания!\n";
}
