#include "../include/route_counter.h"
#include <iostream>

// конструктор: сохраняем ссылку на граф
template <typename T>
RouteCounter<T>::RouteCounter(const Graph<T>& g) 
    : graph(g), numVertices(g.getNumVertices()), routeCount(0) {
    visited.resize(numVertices, false);
}

// рекурсивная функция backtracking для поиска всех маршрутов
// от current до target
template <typename T>
void RouteCounter<T>::backtrack(int current, int target) {
    // добавить текущую вершину в путь
    currentPath.push_back(current);
    
    // базовый случай: достигли целевой вершины
    if (current == target) {
        routeCount++;
        // сохранить найденный маршрут
        allRoutes.push_back(currentPath);
        // убрать вершину из пути перед возвратом
        currentPath.pop_back();
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
    // убрать вершину из текущего пути
    currentPath.pop_back();
}

// подсчитать количество различных маршрутов из start в target
template <typename T>
int RouteCounter<T>::countRoutes(int start, int target) {
    // проверка корректности входных данных
    if (start < 0 || start >= numVertices || target < 0 || target >= numVertices) {
        return 0;
    }
    
    // сброс счётчика и массивов
    routeCount = 0;
    visited.assign(numVertices, false);
    currentPath.clear();
    allRoutes.clear();
    
    // запустить рекурсивный поиск
    backtrack(start, target);
    
    return routeCount;
}

// проверить, существует ли хотя бы один маршрут
template <typename T>
bool RouteCounter<T>::hasRoute(int start, int target) {
    return countRoutes(start, target) > 0;
}

// получить все найденные маршруты
template <typename T>
const std::vector<std::vector<int>>& RouteCounter<T>::getAllRoutes() const {
    return allRoutes;
}

// вывести все найденные маршруты в формате 0->2->3
template <typename T>
void RouteCounter<T>::printAllRoutes() const {
    if (allRoutes.empty()) {
        std::cout << "маршруты не найдены\n";
        return;
    }
    
    std::cout << "\nнайденные маршруты:\n";
    for (size_t i = 0; i < allRoutes.size(); i++) {
        std::cout << "маршрут " << (i + 1) << ": ";
        const auto& route = allRoutes[i];
        for (size_t j = 0; j < route.size(); j++) {
            std::cout << route[j];
            if (j < route.size() - 1) {
                std::cout << "->";
            }
        }
        std::cout << "\n";
    }
}

// явная инстанциация шаблона для double
template class RouteCounter<double>;

