#include "../include/ui.h"
#include <iostream>
#include <limits>
#include "../../common/distributions/normal.h"
#include "../../common/graph/dag_generator.h"
#include "../include/shimbel.h"
#include "../include/route_counter.h"

// параметры нормального распределения для генерации весов рёбер
const double MEAN = 10.0;     // среднее значение (μ)
const double STDDEV = 3.0;    // стандартное отклонение (σ)

// конструктор: инициализируем указатель на граф
UI::UI() : currentGraph(nullptr) {}

// деструктор: освобождаем память
UI::~UI() {
    if (currentGraph != nullptr) {
        delete currentGraph;
    }
}

// очистить буфер ввода
void UI::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// задание 1: генерация связного ациклического графа (DAG)
void UI::task1_generateDAG() {
    std::cout << "\n=== задание 1: генерация связного ациклического графа (DAG) ===\n";
    
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
    
    // генерируем граф
    DAGGenerator<double> generator(&normalDist);
    if (currentGraph != nullptr) {
        delete currentGraph;
    }
    currentGraph = new Graph<double>(generator.generateDAG(numVertices));
    
    std::cout << "\nграф успешно сгенерирован!\n";
    currentGraph->printAdjMatrix();
    currentGraph->printEdges();
}

// задание 2: метод Шимбелла
void UI::task2_shimbel() {
    std::cout << "\n=== задание 2: метод Шимбелла ===\n";
    
    if (currentGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать граф (задание 1)\n";
        return;
    }
    
    int k;
    std::cout << "введите количество рёбер K (длина пути): ";
    std::cin >> k;
    
    if (std::cin.fail() || k <= 0) {
        std::cout << "ошибка: некорректное количество рёбер\n";
        clearInput();
        return;
    }
    
    Shimbel<double> shimbel(*currentGraph);
    
    // находим минимальные пути длины k
    std::cout << "\nвычисление минимальных путей длины " << k << " рёбер...\n";
    auto minPaths = shimbel.findMinPaths(k);
    Shimbel<double>::printPathMatrix(minPaths, "минимальные пути длины " + std::to_string(k));
    
    // находим максимальные пути длины k
    std::cout << "\nвычисление максимальных путей длины " << k << " рёбер...\n";
    auto maxPaths = shimbel.findMaxPaths(k);
    Shimbel<double>::printPathMatrix(maxPaths, "максимальные пути длины " + std::to_string(k));
}

// задание 3: подсчёт маршрутов методом backtracking
void UI::task3_backtracking() {
    std::cout << "\n=== задание 3: подсчёт маршрутов (backtracking) ===\n";
    
    if (currentGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать граф (задание 1)\n";
        return;
    }
    
    int start, target;
    std::cout << "введите начальную вершину S: ";
    std::cin >> start;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    std::cout << "введите конечную вершину T: ";
    std::cin >> target;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный номер вершины\n";
        clearInput();
        return;
    }
    
    // проверка корректности номеров вершин
    int n = currentGraph->getNumVertices();
    if (start < 0 || start >= n || target < 0 || target >= n) {
        std::cout << "ошибка: номера вершин должны быть в диапазоне [0, " << n-1 << "]\n";
        return;
    }
    
    RouteCounter<double> counter(*currentGraph);
    
    // подсчитываем количество маршрутов
    std::cout << "\nподсчёт маршрутов из вершины " << start << " в вершину " << target << "...\n";
    int routeCount = counter.countRoutes(start, target);
    
    if (routeCount > 0) {
        std::cout << "существует маршрут: ДА\n";
        std::cout << "количество различных маршрутов: " << routeCount << "\n";
        
        // вывести все найденные маршруты
        counter.printAllRoutes();
    } else {
        std::cout << "существует маршрут: НЕТ\n";
        std::cout << "количество различных маршрутов: 0\n";
    }
}

// показать главное меню
void UI::showMenu() {
    std::cout << "\n========================================\n";
    std::cout << "лабораторная работа №1: теория графов\n";
    std::cout << "========================================\n";
    std::cout << "1. генерация связного ациклического графа (DAG)\n";
    std::cout << "2. метод Шимбелла\n";
    std::cout << "3. подсчёт маршрутов (backtracking)\n";
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
                task1_generateDAG();
                break;
            case 2:
                task2_shimbel();
                break;
            case 3:
                task3_backtracking();
                break;
            case 0:
                std::cout << "\nзавершение работы программы...\n";
                return;
            default:
                std::cout << "\nошибка: неверный выбор, попробуйте снова\n";
        }
    }
}
