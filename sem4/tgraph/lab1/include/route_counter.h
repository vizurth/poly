#pragma once

#include "common/graph/graph.h"
#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class RouteCounter {
private:
    const Graph<T>& graph;
    int n;
    vector<vector<int>> allRoutes; // все найденные маршруты
    vector<bool> visited; // отметки посещённых вершин
    vector<int> currentPath; // текущий путь
    
    // рекурсивный поиск всех путей от current до target
    void findAllPaths(int current, int target);
    
public:
    RouteCounter(const Graph<T>& g);
    
    // найти все маршруты от source до target
    vector<vector<int>> findAllRoutes(int source, int target);
    
    // вывести все найденные маршруты
    void printRoutes(int source, int target, const vector<vector<int>>& routes);
};

#include "lab1/src/route_counter.cpp"