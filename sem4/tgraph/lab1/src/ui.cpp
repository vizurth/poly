#include "lab1/include/ui.h"
#include <iostream>
#include <limits>

UI::UI() : graphGenerated(false) {}

void UI::printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "лабораторная работа №1: теория графов\n";
    std::cout << "========================================\n";
    std::cout << "1. генерация связного графа\n";
    std::cout << "2. метод Шимбелла\n";
    std::cout << "3. подсчёт маршрутов (backtracking)\n";
    std::cout << "0. выход\n";
    std::cout << "========================================\n";
    std::cout << "выберите действие: ";
}

void UI::task1_generateDAG() {
    std::cout << "\n=== ГЕНЕРАЦИЯ ГРАФА ===\n\n";
    
    int n;
    std::cout << "введите количество вершин N: ";
    std::cin >> n;
    
    if (n <= 0) {
        std::cout << "ошибка: количество вершин должно быть больше 0\n";
        return;
    }
    
    int graphType;
    std::cout << "выберите тип графа:\n";
    std::cout << "1. ориентированный (DAG)\n";
    std::cout << "2. неориентированный\n";
    std::cout << "ваш выбор: ";
    std::cin >> graphType;
    
    bool isDirected = (graphType == 1);
    
    std::cout << "\nпараметры нормального распределения:\n";
    std::cout << "среднее (μ) = 10.0\n";
    std::cout << "стандартное отклонение (σ) = 3.0\n\n";
    
    // создаём распределение
    NormalDistribution dist(10.0, 3.0);
    
    // создаём генератор DAG
    DAGGenerator<double> generator(dist);
    
    // генерируем граф
    graph.reset(new Graph<double>(generator.generateDAG(n, isDirected)));
    graphGenerated = true;
    
    std::cout << (isDirected ? "ориентированный" : "неориентированный") 
              << " граф успешно сгенерирован!\n\n";
    
    // выводим матрицу смежности
    graph->printAdjMatrix();
    
    // выводим список рёбер
    std::cout << "\nсписок рёбер:\n";
    graph->printEdges();
}

void UI::task2_shimbel() {
    if (!graphGenerated) {
        std::cout << "\nошибка: сначала сгенерируйте граф (задание 1)\n";
        return;
    }
    
    std::cout << "\n=== МЕТОД ШИМБЕЛЛА ===\n\n";
    
    int K;
    std::cout << "введите количество рёбер K (длина пути): ";
    std::cin >> K;
    
    if (K <= 0) {
        std::cout << "ошибка: K должно быть больше 0\n";
        return;
    }
    
    ShimbelSolver<double> solver(*graph);
    
    // вычисляем кратчайшие пути
    auto shortest = solver.computeShortestPaths(K);
    ShimbelSolver<double>::printMatrix(shortest, "Матрица Шимбелла (мин) k = " + std::to_string(K) + ":");
    
    // вычисляем длиннейшие пути
    auto longest = solver.computeLongestPaths(K);
    ShimbelSolver<double>::printMatrix(longest, "Матрица Шимбелла (макс) k = " + std::to_string(K) + ":");
}

void UI::task3_routeCounter() {
    if (!graphGenerated) {
        std::cout << "\nошибка: сначала сгенерируйте граф (задание 1)\n";
        return;
    }
    
    std::cout << "\n=== ПОДСЧЁТ МАРШРУТОВ ===\n\n";
    
    int source, target;
    std::cout << "введите начальную вершину S: ";
    std::cin >> source;
    std::cout << "введите конечную вершину T: ";
    std::cin >> target;
    
    int n = graph->getNumVertices();
    if (source < 0 || source >= n || target < 0 || target >= n) {
        std::cout << "ошибка: вершины должны быть в диапазоне [0, " << (n - 1) << "]\n";
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
        std::cin >> choice;
        
        // проверка на ошибку ввода
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nошибка: введите число\n";
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
                std::cout << "\nвыход из программы...\n";
                return;
            default:
                std::cout << "\nошибка: неверный выбор\n";
        }
    }
}
