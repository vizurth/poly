#ifndef ROUTE_COUNTER_H
#define ROUTE_COUNTER_H

#include "../../common/graph/graph.h"
#include <vector>

// класс для подсчёта маршрутов методом backtracking
class RouteCounter {
private:
    const Graph& graph;
    int numVertices;
    int routeCount;  // счётчик найденных маршрутов
    std::vector<bool> visited;  // посещённые вершины в текущем пути
    
    // рекурсивная функция для поиска маршрутов с возвратом (backtracking)
    // ищет все пути из текущей вершины current в целевую вершину target
    void backtrack(int current, int target);
    
public:
    // конструктор: принимает граф для анализа
    RouteCounter(const Graph& g);
    
    // подсчитать количество маршрутов из вершины start в вершину target
    // возвращает количество найденных маршрутов
    int countRoutes(int start, int target);
    
    // проверить, существует ли хотя бы один маршрут из start в target
    bool hasRoute(int start, int target);
};

#endif // ROUTE_COUNTER_H
