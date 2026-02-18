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
    
    cout << "\nпараметры нормального распределения:\n";
    cout << "среднее (μ) = 10.0\n";
    cout << "стандартное отклонение (σ) = 3.0\n\n";
    
    // создаём распределение
    NormalDistribution dist(10.0, 3.0);
    
    // создаём генератор DAG
    DAGGenerator<double> generator(dist);
    
    // генерируем граф
    graph.reset(new Graph<double>(generator.generateDAG(n, isDirected)));
    graphGenerated = true;
    
    cout << (isDirected ? "ориентированный" : "неориентированный") 
              << " граф успешно сгенерирован!\n\n";
    
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
