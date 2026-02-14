#include "../include/ui.h"
#include "../include/euler.h"
#include "../include/hamilton.h"
#include "../include/tsp.h"
#include "../../common/distributions/normal.h"
#include "../../common/graph/dag_generator.h"
#include <iostream>
#include <limits>

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

// задание 1: проверка графа на эйлеровость и гамильтоновость
void UI::task1_checkProperties() {
    std::cout << "\n=== задание 1: проверка графа на эйлеровость и гамильтоновость ===\n";
    
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
    
    // проверка эйлеровости
    EulerGraph<double> eulerGraph(currentGraph);
    std::cout << "\n--- проверка эйлеровости ---\n";
    
    if (eulerGraph.isEulerian()) {
        std::cout << "граф является эйлеровым (существует эйлеров цикл)\n";
    } else if (eulerGraph.hasEulerianPath()) {
        std::cout << "граф имеет эйлеров путь (но не цикл)\n";
    } else {
        std::cout << "граф НЕ является эйлеровым\n";
    }
    
    eulerGraph.printDegrees();
    
    // проверка гамильтоновости
    HamiltonGraph<double> hamiltonGraph(currentGraph);
    std::cout << "\n--- проверка гамильтоновости ---\n";
    
    if (hamiltonGraph.isHamiltonian()) {
        std::cout << "граф является гамильтоновым (существует гамильтонов цикл)\n";
        
        auto cycle = hamiltonGraph.findHamiltonianCycle();
        std::cout << "гамильтонов цикл: ";
        for (int v : cycle) {
            std::cout << v << " ";
        }
        std::cout << "-> " << cycle[0] << "\n";
    } else {
        std::cout << "граф НЕ является гамильтоновым\n";
    }
}

// задание 2: эйлеров граф и эйлеров цикл
void UI::task2_eulerianCircuit() {
    std::cout << "\n=== задание 2: эйлеров граф и эйлеров цикл ===\n";
    
    if (currentGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать граф (задание 1)\n";
        return;
    }
    
    EulerGraph<double> eulerGraph(currentGraph);
    
    // проверить, является ли граф эйлеровым
    if (!eulerGraph.isEulerian()) {
        std::cout << "граф не является эйлеровым. модифицируем его...\n";
        
        auto addedEdges = eulerGraph.makeEulerian();
        
        if (!addedEdges.empty()) {
            std::cout << "\nдобавлены следующие рёбра:\n";
            for (const auto& edge : addedEdges) {
                std::cout << "  " << edge.first << " -> " << edge.second << "\n";
            }
            
            std::cout << "\nобновлённая матрица смежности:\n";
            currentGraph->printAdjMatrix();
        }
    } else {
        std::cout << "граф уже является эйлеровым\n";
    }
    
    // построить эйлеров цикл
    std::cout << "\nпостроение эйлерова цикла (алгоритм хирхольцера)...\n";
    auto circuit = eulerGraph.findEulerianCircuit();
    
    if (!circuit.empty()) {
        std::cout << "эйлеров цикл найден:\n";
        for (size_t i = 0; i < circuit.size(); i++) {
            std::cout << circuit[i];
            if (i < circuit.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    } else {
        std::cout << "не удалось построить эйлеров цикл\n";
    }
}

// задание 3: гамильтонов граф и задача коммивояжера
void UI::task3_tsp() {
    std::cout << "\n=== задание 3: гамильтонов граф и задача коммивояжера ===\n";
    
    if (currentGraph == nullptr) {
        std::cout << "ошибка: сначала необходимо сгенерировать граф (задание 1)\n";
        return;
    }
    
    HamiltonGraph<double> hamiltonGraph(currentGraph);
    
    // проверить, является ли граф гамильтоновым
    if (!hamiltonGraph.isHamiltonian()) {
        std::cout << "граф не является гамильтоновым. модифицируем его...\n";
        
        auto addedEdges = hamiltonGraph.makeHamiltonian();
        
        if (!addedEdges.empty()) {
            std::cout << "\nдобавлены следующие рёбра:\n";
            for (const auto& edge : addedEdges) {
                std::cout << "  " << edge.first << " -> " << edge.second << "\n";
            }
            
            std::cout << "\nобновлённая матрица смежности:\n";
            currentGraph->printAdjMatrix();
        }
    } else {
        std::cout << "граф уже является гамильтоновым\n";
    }
    
    // решить задачу коммивояжера
    std::cout << "\n--- задача коммивояжера (TSP) ---\n";
    std::cout << "выберите метод решения:\n";
    std::cout << "1. полный перебор (найти все гамильтоновы циклы)\n";
    std::cout << "2. жадный алгоритм (ближайший сосед)\n";
    std::cout << "3. 2-opt улучшение\n";
    
    int choice;
    std::cout << "ваш выбор: ";
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cout << "ошибка: некорректный ввод\n";
        clearInput();
        return;
    }
    
    TSP<double> tsp(currentGraph);
    
    switch (choice) {
        case 1: {
            std::cout << "\nполный перебор всех гамильтоновых циклов...\n";
            auto result = tsp.solveBruteForce();
            
            const auto& allCycles = tsp.getAllCycles();
            std::cout << "всего найдено циклов: " << allCycles.size() << "\n";
            
            if (!result.path.empty()) {
                std::cout << "\nлучший цикл:\n";
                std::cout << "стоимость: " << result.cost << "\n";
                std::cout << "путь: ";
                for (int v : result.path) {
                    std::cout << v << " ";
                }
                std::cout << "-> " << result.path[0] << "\n";
                
                if (allCycles.size() < 20) {
                    std::cout << "\nвсе найденные циклы:\n";
                    tsp.printTopCycles(allCycles.size());
                } else {
                    std::cout << "\nслишком много циклов для вывода на экран\n";
                    tsp.writeCyclesToFile("results.txt");
                    tsp.printTopCycles(10);
                }
            } else {
                std::cout << "гамильтоновы циклы не найдены\n";
            }
            break;
        }
        
        case 2: {
            std::cout << "\nжадный алгоритм (ближайший сосед)...\n";
            auto result = tsp.solveGreedy(0);
            
            if (!result.path.empty()) {
                std::cout << "найденный цикл:\n";
                std::cout << "стоимость: " << result.cost << "\n";
                std::cout << "путь: ";
                for (int v : result.path) {
                    std::cout << v << " ";
                }
                std::cout << "-> " << result.path[0] << "\n";
            } else {
                std::cout << "не удалось найти гамильтонов цикл\n";
            }
            break;
        }
        
        case 3: {
            std::cout << "\n2-opt улучшение (начиная с жадного решения)...\n";
            auto result = tsp.solve2Opt(0);
            
            if (!result.path.empty()) {
                std::cout << "улучшенный цикл:\n";
                std::cout << "стоимость: " << result.cost << "\n";
                std::cout << "путь: ";
                for (int v : result.path) {
                    std::cout << v << " ";
                }
                std::cout << "-> " << result.path[0] << "\n";
            } else {
                std::cout << "не удалось найти гамильтонов цикл\n";
            }
            break;
        }
        
        default:
            std::cout << "ошибка: неверный выбор\n";
    }
}

// показать главное меню
void UI::showMenu() {
    std::cout << "\n========================================\n";
    std::cout << "лабораторная работа №5: эйлеровы и гамильтоновы графы\n";
    std::cout << "========================================\n";
    std::cout << "1. проверка графа на эйлеровость и гамильтоновость\n";
    std::cout << "2. эйлеров граф и эйлеров цикл\n";
    std::cout << "3. гамильтонов граф и задача коммивояжера (TSP)\n";
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
                task1_checkProperties();
                break;
            case 2:
                task2_eulerianCircuit();
                break;
            case 3:
                task3_tsp();
                break;
            case 0:
                std::cout << "\nзавершение работы программы...\n";
                return;
            default:
                std::cout << "\nошибка: неверный выбор, попробуйте снова\n";
        }
    }
}
