#ifndef ROUTE_COUNTER_H
#define ROUTE_COUNTER_H

#include "common/graph/graph.h"
#include <vector>
#include <iostream>

template <typename T>
class RouteCounter {
private:
    const Graph<T>& graph;
    int n; // количество вершин
    std::vector<std::vector<int>> allRoutes; // все найденные маршруты
    std::vector<bool> visited; // отметки посещённых вершин
    std::vector<int> currentPath; // текущий путь
    
    // рекурсивный поиск всех путей от current до target
    void findAllPaths(int current, int target);
    
public:
    explicit RouteCounter(const Graph<T>& g);
    
    // найти все маршруты от source до target
    std::vector<std::vector<int>> findAllRoutes(int source, int target);
    
    // вывести все найденные маршруты
    void printRoutes(int source, int target, const std::vector<std::vector<int>>& routes);
};

#include "lab1/src/route_counter.cpp"

#endif // ROUTE_COUNTER_H
