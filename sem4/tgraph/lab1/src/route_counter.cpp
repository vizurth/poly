#include "lab1/include/route_counter.h"

template <typename T>
RouteCounter<T>::RouteCounter(const Graph<T>& g) : graph(g), n(g.getNumVertices()) {
    visited.resize(n, false);
}

template <typename T>
void RouteCounter<T>::findAllPaths(int current, int target) {
    // добавляем текущую вершину в путь
    currentPath.push_back(current);
    visited[current] = true;
    
    // если достигли целевой вершины, сохраняем путь
    if (current == target) {
        allRoutes.push_back(currentPath);
    } else {
        // пробуем все исходящие рёбра
        auto outgoing = graph.getOutgoingEdges(current);
        for (const auto& edge : outgoing) {
            int next = edge.first;
            // если вершина не посещена, идём в неё
            if (!visited[next]) {
                findAllPaths(next, target);
            }
        }
    }
    
    // откатываем изменения (backtracking)
    visited[current] = false;
    currentPath.pop_back();
}

template <typename T>
std::vector<std::vector<int>> RouteCounter<T>::findAllRoutes(int source, int target) {
    // очищаем предыдущие результаты
    allRoutes.clear();
    currentPath.clear();
    std::fill(visited.begin(), visited.end(), false);
    
    // проверяем корректность вершин
    if (source < 0 || source >= n || target < 0 || target >= n) {
        return allRoutes;
    }
    
    // запускаем поиск
    findAllPaths(source, target);
    
    return allRoutes;
}

template <typename T>
void RouteCounter<T>::printRoutes(int source, int target, const std::vector<std::vector<int>>& routes) {
    std::cout << "\n=== поиск маршрутов от вершины " << source << " до вершины " << target << " ===\n\n";
    
    if (routes.empty()) {
        std::cout << "существует маршрут: НЕТ\n";
        std::cout << "количество маршрутов: 0\n";
        return;
    }
    
    std::cout << "существует маршрут: ДА\n";
    std::cout << "количество маршрутов: " << routes.size() << "\n\n";
    std::cout << "все найденные маршруты:\n";
    
    for (size_t i = 0; i < routes.size(); i++) {
        std::cout << "  маршрут " << (i + 1) << ": ";
        for (size_t j = 0; j < routes[i].size(); j++) {
            std::cout << routes[i][j];
            if (j < routes[i].size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}