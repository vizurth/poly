#include "lab1/include/route_counter.h"
#include "common/graph/graph.h"
#include <iostream>

int main() {
    std::cout << "=== ТЕСТ ПОДСЧЁТА МАРШРУТОВ (BACKTRACKING) ===\n\n";
    
    // тест 1: простой граф с несколькими путями
    std::cout << "тест 1: граф с 5 вершинами и несколькими путями\n";
    std::cout << "структура графа:\n";
    std::cout << "  0 -> 1 (2.0)\n";
    std::cout << "  0 -> 2 (5.0)\n";
    std::cout << "  1 -> 2 (1.0)\n";
    std::cout << "  1 -> 3 (4.0)\n";
    std::cout << "  2 -> 3 (2.0)\n";
    std::cout << "  2 -> 4 (3.0)\n";
    std::cout << "  3 -> 4 (1.0)\n\n";
    
    Graph<double> graph1(5);
    graph1.addEdge(0, 1, 2.0);
    graph1.addEdge(0, 2, 5.0);
    graph1.addEdge(1, 2, 1.0);
    graph1.addEdge(1, 3, 4.0);
    graph1.addEdge(2, 3, 2.0);
    graph1.addEdge(2, 4, 3.0);
    graph1.addEdge(3, 4, 1.0);
    
    graph1.printAdjMatrix();
    
    RouteCounter<double> counter1(graph1);
    
    // пути от 0 до 3
    std::cout << "\n--- поиск путей от 0 до 3 ---\n";
    auto routes_0_3 = counter1.findAllRoutes(0, 3);
    counter1.printRoutes(0, 3, routes_0_3);
    
    std::cout << "ожидаемые пути:\n";
    std::cout << "  1. 0 -> 1 -> 3\n";
    std::cout << "  2. 0 -> 2 -> 3\n";
    std::cout << "  3. 0 -> 1 -> 2 -> 3\n\n";
    
    // пути от 0 до 4
    std::cout << "\n--- поиск путей от 0 до 4 ---\n";
    auto routes_0_4 = counter1.findAllRoutes(0, 4);
    counter1.printRoutes(0, 4, routes_0_4);
    
    std::cout << "ожидаемые пути:\n";
    std::cout << "  1. 0 -> 2 -> 4\n";
    std::cout << "  2. 0 -> 1 -> 3 -> 4\n";
    std::cout << "  3. 0 -> 2 -> 3 -> 4\n";
    std::cout << "  4. 0 -> 1 -> 2 -> 4\n";
    std::cout << "  5. 0 -> 1 -> 2 -> 3 -> 4\n\n";
    
    // тест 2: нет пути
    std::cout << "\n=== тест 2: проверка отсутствия пути ===\n";
    std::cout << "граф: 0 -> 1, 2 -> 3 (несвязные компоненты)\n\n";
    
    Graph<double> graph2(4);
    graph2.addEdge(0, 1, 1.0);
    graph2.addEdge(2, 3, 1.0);
    
    RouteCounter<double> counter2(graph2);
    auto routes_0_3_nopath = counter2.findAllRoutes(0, 3);
    counter2.printRoutes(0, 3, routes_0_3_nopath);
    
    // тест 3: путь к самой себе
    std::cout << "\n=== тест 3: путь от вершины к себе же ===\n";
    auto routes_self = counter1.findAllRoutes(2, 2);
    counter1.printRoutes(2, 2, routes_self);
    
    std::cout << "=== ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ ===\n";
    
    return 0;
}
