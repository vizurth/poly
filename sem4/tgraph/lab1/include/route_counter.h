#ifndef ROUTE_COUNTER_H
#define ROUTE_COUNTER_H

#include "../../common/graph/graph.h"
#include <vector>
#include <string>

// класс для подсчёта маршрутов методом backtracking
// T - тип весов рёбер
template <typename T = double>
class RouteCounter {
private:
    const Graph<T>& graph;
    int numVertices;
    int routeCount;  // счётчик найденных маршрутов
    std::vector<bool> visited;  // посещённые вершины в текущем пути
    std::vector<int> currentPath;  // текущий путь (для вывода)
    std::vector<std::vector<int>> allRoutes;  // все найденные маршруты
    
    // рекурсивная функция для поиска маршрутов с возвратом (backtracking)
    // ищет все пути из текущей вершины current в целевую вершину target
    void backtrack(int current, int target);
    
public:
    // конструктор: принимает граф для анализа
    RouteCounter(const Graph<T>& g);
    
    // подсчитать количество маршрутов из вершины start в вершину target
    // возвращает количество найденных маршрутов
    int countRoutes(int start, int target);
    
    // проверить, существует ли хотя бы один маршрут из start в target
    bool hasRoute(int start, int target);
    
    // получить все найденные маршруты
    const std::vector<std::vector<int>>& getAllRoutes() const;
    
    // вывести все найденные маршруты в формате 0->2->3
    void printAllRoutes() const;
};

#endif // ROUTE_COUNTER_H
