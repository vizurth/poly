#include "../include/ui.h"
#include "../include/capacity_generator.h"
#include "../include/ford_fulkerson.h"
#include "../include/min_cost_flow.h"
#include "../../common/distributions/normal.h"
#include "../../common/graph/dag_generator.h"
#include <iostream>
#include <limits>

// параметры нормального распределения для генерации весов рёбер
const double MEAN = 10.0;     // среднее значение (μ)
const double STDDEV = 3.0;    // стандартное отклонение (σ)

// конструктор: инициализируем указатели
UI::UI() : currentGraph(nullptr), capacityGraph(nullptr), costGraph(nullptr), maxFlowValue(0) {}

// деструктор: освобождаем память
UI::~UI() {
    if (currentGraph != nullptr) {
        delete currentGraph;
    }
    if (capacityGraph != nullptr) {
        delete capacityGraph;
    }
    if (costGraph != nullptr) {
        delete costGraph;
    }
}

// очистить буфер ввода
void UI::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// задание 1: генерация матриц пропускных способностей и стоимостей
void UI::task1_generateMatrices() {
    std::cout << "\n=== задание 1: генерация матриц пропускных способностей и стоимостей ===\n";
    
    int numVertices;
    std::cout << "введите количество вершин N: ";
    std::cin >> numVertices;
    
    if (std::cin.fail() || numVertices <= 0) {
        std::cout << "ошибка: некорректное количество вершин\n";
        clearInput();
        return;
    }
    
    // создаём нормальное распределение с заданными параметрами
    NormalDistribution normalDist(MEAN, STDDEV);
    std::cout << "\nпараметры нормального распределения:\n";
    std::cout << "среднее (μ) = " << MEAN << "\n";
    std::cout << "стандартное отклонение (σ) = " << STDDEV << "\n";
    
    // генерируем базовый DAG
    DAGGenerator<double> generator(&normalDist);
    if (currentGraph != nullptr) {
        delete currentGraph;
    }
    currentGraph = new Graph<double>(generator.generateDAG(numVertices));
    
    std::cout << "\nбазовый граф (DAG) успешно сгенерирован!\n";
    currentGraph->printAdjMatrix();
    currentGraph->printEdges();
    
    // генерируем матрицы пропускных способностей и стоимостей
    CapacityGenerator capGen(&normalDist);
    auto matrices = capGen.generateMatrices(*currentGraph);
    
    if (capacityGraph != nullptr) {
        delete capacityGraph;
    }
    if (costGraph != nullptr) {
        delete costGraph;
    }
    
    capacityGraph = new Graph<int>(matrices.first);
    costGraph = new Graph<double>(matrices.second);
    
    std::cout << "\n--- матрица пропускных способностей (целые числа) ---\n";
    capacityGraph->printAdjMatrix();
    
    std::cout << "\n--- матрица стоимостей (вещественные числа) ---\n";
    costGraph->printAdjMatrix();
    
    std::cout << "\nматрицы успешно сгенерированы!\n";
}

// задание 2: нахождение максимального потока (алгоритм Форда-Фалкерсона)
void UI::task2_maxFlow() {
    std::cout << "\n=== задание 2: максимальный поток (алгоритм Форда-Фалкерсона) ===\n";
    
    if (capacityGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать матрицы (задание 1)\n";
        return;
    }
    
    int source, sink;
    std::cout << "введите исток (начальная вершина): ";
    std::cin >> source;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    std::cout << "введите сток (конечная вершина): ";
    std::cin >> sink;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    // проверка корректности номеров вершин
    int n = capacityGraph->getNumVertices();
    if (source < 0 || source >= n || sink < 0 || sink >= n) {
        std::cout << "ошибка: номера вершин должны быть в диапазоне [0, " << n-1 << "]\n";
        return;
    }
    
    if (source == sink) {
        std::cout << "ошибка: исток и сток должны быть разными вершинами\n";
        return;
    }
    
    // находим максимальный поток
    FordFulkerson ff(n);
    maxFlowValue = ff.maxFlow(*capacityGraph, source, sink);
    
    std::cout << "\n--- результаты алгоритма Форда-Фалкерсона ---\n";
    std::cout << "максимальный поток от вершины " << source << " к вершине " << sink << ": " << maxFlowValue << "\n";
    ff.printFlow();
}

// задание 3: нахождение потока минимальной стоимости величиной 2/3 от максимального
void UI::task3_minCostFlow() {
    std::cout << "\n=== задание 3: поток минимальной стоимости ===\n";
    
    if (capacityGraph == nullptr || costGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать матрицы (задание 1)\n";
        return;
    }
    
    if (maxFlowValue == 0) {
        std::cout << "ошибка: сначала необходимо найти максимальный поток (задание 2)\n";
        return;
    }
    
    int source, sink;
    std::cout << "введите исток (начальная вершина): ";
    std::cin >> source;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    std::cout << "введите сток (конечная вершина): ";
    std::cin >> sink;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    // проверка корректности номеров вершин
    int n = capacityGraph->getNumVertices();
    if (source < 0 || source >= n || sink < 0 || sink >= n) {
        std::cout << "ошибка: номера вершин должны быть в диапазоне [0, " << n-1 << "]\n";
        return;
    }
    
    if (source == sink) {
        std::cout << "ошибка: исток и сток должны быть разными вершинами\n";
        return;
    }
    
    // вычисляем 2/3 от максимального потока
    int targetFlow = (maxFlowValue * 2) / 3;
    std::cout << "\nмаксимальный поток: " << maxFlowValue << "\n";
    std::cout << "целевая величина потока (2/3 от максимального): " << targetFlow << "\n";
    
    // находим поток минимальной стоимости
    MinCostFlow mcf(n);
    double totalCost = mcf.minCostMaxFlow(*capacityGraph, *costGraph, source, sink, targetFlow);
    
    std::cout << "\n--- результаты алгоритма минимальной стоимости ---\n";
    mcf.printFlow(totalCost);
}

// показать главное меню
void UI::showMenu() {
    std::cout << "\n========================================\n";
    std::cout << "лабораторная работа №3: потоки в сетях\n";
    std::cout << "========================================\n";
    std::cout << "1. генерация матриц пропускных способностей и стоимостей\n";
    std::cout << "2. нахождение максимального потока (Форд-Фалкерсон)\n";
    std::cout << "3. нахождение потока минимальной стоимости (2/3 от макс.)\n";
    std::cout << "0. выход\n";
    std::cout << "========================================\n";
    std::cout << "выберите действие: ";
}

// запустить главный цикл программы
void UI::run() {
    int choice;
    
    while (true) {
        showMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cout << "\nошибка: введите число\n";
            clearInput();
            continue;
        }
        
        switch (choice) {
            case 1:
                task1_generateMatrices();
                break;
            case 2:
                task2_maxFlow();
                break;
            case 3:
                task3_minCostFlow();
                break;
            case 0:
                std::cout << "\nзавершение работы программы...\n";
                return;
            default:
                std::cout << "\nошибка: неверный выбор, попробуйте снова\n";
        }
    }
}
