#include "lab1/src/ui.h"
#include "lab2/src/ui.h"
#include "lab3/src/ui.h"
#include "lab4/src/ui.h"
#include "lab5/src/ui.h"
#include <iostream>
#include <limits>

// TODO: проверить если пропускная способность при 2 вершинах равно 1 то правильно ли отрабатывает целевой поток

using namespace std;

static int askNumVertices() {
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

static bool askDirected() {
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

static WeightType askWeightType() {
	int choice;
	cout << "\n  Тип весов рёбер:\n";
	cout << "    1. Только положительные\n";
	cout << "    2. Только отрицательные\n";
	cout << "    3. Смешанные\n";
	while (true) {
		cout << "  Ваш выбор [1-3]: ";
		if (cin >> choice && choice >= 1 && choice <= 3) {
			if (choice == 1) return WeightType::POSITIVE;
			if (choice == 2) return WeightType::NEGATIVE;
			return WeightType::MIXED;
		}
		cout << "  Ошибка: введите число от 1 до 3.\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

int main() {
	Lab1UI lab1;
	Lab2UI lab2;
	Lab3UI lab3;
	Lab4UI lab4;
	Lab5UI lab5;

	Graph<int> *graph = nullptr;

	int choice;
	do {
		cout << "\n == Меню ==\n";

		cout << "\n  ОБЩЕЕ\n";
		cout << "    1. Сгенерировать граф\n";
		cout << "    2. Вывести матрицу смежности\n";
		cout << "    3. Вывести матрицу весов\n";

		cout << "\n  ЛАБОРАТОРНАЯ РАБОТА № 1\n";
		cout << "    4. Вычислить эксцентриситеты вершин\n";
		cout << "    5. Найти центр графа\n";
		cout << "    6. Найти диаметр графа\n";
		cout << "    7. Матрица Шимбела\n";
		cout << "    8. Найти все пути от A до B\n";

		cout << "\n  ЛАБОРАТОРНАЯ РАБОТА № 2\n";
		cout << "    9.  Топологическая сортировка (алгоритм Фалкерсона)\n";
		cout << "    10. Кратчайшие пути (алгоритм Флойда-Уоршалла)\n";
		cout << "    11. Сравнить алгоритмы по числу итераций\n";

		cout << "\n  ЛАБОРАТОРНАЯ РАБОТА № 3\n";
		cout << "    12. Вывести матрицу пропускных способностей\n";
		cout << "    13. Вывести матрицу стоимости\n";
		cout << "    14. Максимальный поток\n";
		cout << "    15. Поток минимальной стоимости [2/3 * max]\n";

		cout << "\n  ЛАБОРАТОРНАЯ РАБОТА № 4\n";
		cout << "    16. Число остовных деревьев (теорема Кирхгофа)\n";
		cout << "    17. Минимальный остов (алгоритм Краскала)\n";
		cout << "    18. Кодировать остов кодом Прюфера\n";
		cout << "    19. Декодировать код Прюфера\n";
		cout << "    20. Максимальное независимое множество\n";

		cout << "\n  ЛАБОРАТОРНАЯ РАБОТА № 5\n";
		cout << "    21. Проверить эйлеровость, модифицировать и построить цикл\n";

		cout << "\n     0. Выход\n";
		cout << "  Ваш выбор: ";

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (choice == 0)
			break;

		if (choice == 1) {
			int n        = askNumVertices();
			bool directed = askDirected();
			WeightType wt = askWeightType();
			try {
				delete graph;
				graph = new Graph<int>(
				    Generator<int>(n, directed, wt, 10.0, 2.0, 10.0, 2.0)
				        .generateGraph());
				lab3.onNewGraph(*graph, n);
				lab4.onNewGraph(directed);
				lab5.onNewGraph(directed);
				cout << "\n  Граф сгенерирован (" << n << " вершин).\n";
			} catch (const exception &e) {
				cout << "  Ошибка генерации: " << e.what() << "\n";
			}
		} else if (choice == 2) {
			if (!graph)
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			else
				graph->printAdjMatrix();
		} else if (choice == 3) {
			if (!graph)
				cout << "  Сначала сгенерируйте граф (пункт 1).\n";
			else {
				graph->printWeightMatrix();
				graph->printEdges();
			}
		} else if (choice >= 4 && choice <= 8) {
			lab1.processChoice(choice - 3, graph);
		} else if (choice >= 9 && choice <= 11) {
			lab2.processChoice(choice - 8, graph);
		} else if (choice >= 12 && choice <= 15) {
			lab3.processChoice(choice - 11, graph);
		} else if (choice >= 16 && choice <= 20) {
			lab4.processChoice(choice - 15, graph);
		} else if (choice == 21) {
			lab5.processChoice(1, graph);
		} else {
			cout << "  Неверный выбор.\n";
		}

	} while (true);

	delete graph;
	cout << "\nДо свидания!\n";
	return 0;
}
