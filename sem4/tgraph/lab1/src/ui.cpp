#include "lab1/include/ui.h"
#include <iostream>
#include <limits>

UI::UI() : graphGenerated(false) {}

void UI::printMenu() {
    cout << "\n========================================\n";
    cout << "лабораторная работа №1: теория графов\n";
    cout << "========================================\n";
    cout << "1. генерация связного графа\n";
    cout << "2. метод Шимбелла\n";
    cout << "3. подсчёт маршрутов (backtracking)\n";
    cout << "0. выход\n";
    cout << "========================================\n";
    cout << "выберите действие: ";
}

void UI::task1_generateDAG() {
    cout << "\n=== ГЕНЕРАЦИЯ ГРАФА ===\n\n";
    
    int n;
    cout << "введите количество вершин N: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "ошибка: количество вершин должно быть больше 0\n";
        return;
    }
    
    int graphType;
    cout << "выберите тип графа:\n";
    cout << "1. ориентированный (DAG)\n";
    cout << "2. неориентированный\n";
    cout << "ваш выбор: ";
    cin >> graphType;
    
    bool isDirected = (graphType == 1);
    
    int weightChoice;
    cout << "\nвыберите тип весов рёбер:\n";
    cout << "1. положительные\n";
    cout << "2. отрицательные\n";
    cout << "3. смешанные (положительные и отрицательные)\n";
    cout << "ваш выбор: ";
    cin >> weightChoice;
    
    WeightType weightType;
    switch (weightChoice) {
        case 1: weightType = WeightType::POSITIVE; break;
        case 2: weightType = WeightType::NEGATIVE; break;
        case 3: weightType = WeightType::MIXED; break;
        default:
            cout << "ошибка: неверный выбор, используем положительные веса\n";
            weightType = WeightType::POSITIVE;
    }
    
    cout << "\nпараметры распределения Вейбулла:\n";
    cout << "масштаб (a) = 10.0\n";
    cout << "форма (c) = 2.0\n\n";
    
    // создаём распределение Вейбулла
    WeibullDistribution dist(10.0, 2.0);
    
    // создаём генератор DAG
    DAGGenerator<double> generator(dist);
    
    // генерируем граф с выбранным типом весов
    graph.reset(new Graph<double>(generator.generateDAG(n, isDirected, weightType)));
    graphGenerated = true;
    
    cout << (isDirected ? "ориентированный" : "неориентированный") 
              << " граф с ";
    switch (weightType) {
        case WeightType::POSITIVE: cout << "положительными"; break;
        case WeightType::NEGATIVE: cout << "отрицательными"; break;
        case WeightType::MIXED: cout << "смешанными"; break;
    }
    cout << " весами успешно сгенерирован!\n\n";
    
    // выводим матрицу смежности
    graph->printAdjMatrix();
    
    // выводим список рёбер
    cout << "\nсписок рёбер:\n";
    graph->printEdges();
}

void UI::task2_shimbel() {
    if (!graphGenerated) {
        cout << "\nошибка: сначала сгенерируйте граф (задание 1)\n";
        return;
    }
    
    cout << "\n=== МЕТОД ШИМБЕЛЛА ===\n\n";
    
    int K;
    cout << "введите количество рёбер K (длина пути): ";
    cin >> K;
    
    if (K <= 0) {
        cout << "ошибка: K должно быть больше 0\n";
        return;
    }
    
    ShimbelSolver<double> solver(*graph);
    
    // вычисляем кратчайшие пути
    auto shortest = solver.computeShortestPaths(K);
    ShimbelSolver<double>::printMatrix(shortest, "Матрица Шимбелла (мин) k = " + to_string(K) + ":");
    
    // вычисляем длиннейшие пути
    auto longest = solver.computeLongestPaths(K);
    ShimbelSolver<double>::printMatrix(longest, "Матрица Шимбелла (макс) k = " + to_string(K) + ":");
}

void UI::task3_routeCounter() {
    if (!graphGenerated) {
        cout << "\nошибка: сначала сгенерируйте граф (задание 1)\n";
        return;
    }
    
    cout << "\n=== ПОДСЧЁТ МАРШРУТОВ ===\n\n";
    
    int source, target;
    cout << "введите начальную вершину S: ";
    cin >> source;
    cout << "введите конечную вершину T: ";
    cin >> target;
    
    int n = graph->getNumVertices();
    if (source < 0 || source >= n || target < 0 || target >= n) {
        cout << "ошибка: вершины должны быть в диапазоне [0, " << (n - 1) << "]\n";
        return;
    }
    
    RouteCounter<double> counter(*graph);
    auto routes = counter.findAllRoutes(source, target);
    counter.printRoutes(source, target, routes);
}

void UI::run() {
    int choice;
    
    while (true) {
        printMenu();
        cin >> choice;
        
        // проверка на ошибку ввода
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nошибка: введите число\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                task1_generateDAG();
                break;
            case 2:
                task2_shimbel();
                break;
            case 3:
                task3_routeCounter();
                break;
            case 0:
                cout << "\nвыход из программы...\n";
                return;
            default:
                cout << "\nошибка: неверный выбор\n";
        }
    }
}
