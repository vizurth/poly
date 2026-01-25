#include "../include/route_counter.h"

// конструктор: сохраняем ссылку на граф
RouteCounter::RouteCounter(const Graph& g) 
    : graph(g), numVertices(g.getNumVertices()), routeCount(0) {
    visited.resize(numVertices, false);
}

// рекурсивная функция backtracking для поиска всех маршрутов
// от current до target
void RouteCounter::backtrack(int current, int target) {
    // базовый случай: достигли целевой вершины
    if (current == target) {
        routeCount++;
        return;
    }
    
    // отметить текущую вершину как посещённую
    visited[current] = true;
    
    // попробовать перейти во все соседние вершины
    for (int next = 0; next < numVertices; next++) {
        // если есть ребро в следующую вершину и она не посещена
        if (graph.hasEdge(current, next) && !visited[next]) {
            // рекурсивно продолжить поиск из следующей вершины
            backtrack(next, target);
        }
    }
    
    // откатить изменения (backtrack): убрать отметку о посещении
    // это позволяет использовать эту вершину в других маршрутах
    visited[current] = false;
}

// подсчитать количество различных маршрутов из start в target
int RouteCounter::countRoutes(int start, int target) {
    // проверка корректности входных данных
    if (start < 0 || start >= numVertices || target < 0 || target >= numVertices) {
        return 0;
    }
    
    // сброс счётчика и массива посещений
    routeCount = 0;
    visited.assign(numVertices, false);
    
    // запустить рекурсивный поиск
    backtrack(start, target);
    
    return routeCount;
}

// проверить, существует ли хотя бы один маршрут
bool RouteCounter::hasRoute(int start, int target) {
    return countRoutes(start, target) > 0;
}
