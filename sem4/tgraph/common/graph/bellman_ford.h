#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "graph.h"
#include <vector>
#include <limits>

// алгоритм Беллмана-Форда для поиска кратчайших путей
// работает с графами, содержащими рёбра с отрицательными весами
// может обнаруживать отрицательные циклы
template <typename T = double>
class BellmanFord {
private:
    const Graph<T>& graph;
    int numVertices;
    const T INF = std::numeric_limits<T>::max();
    
public:
    // конструктор: принимает граф для анализа
    BellmanFord(const Graph<T>& g);
    
    // найти кратчайшие пути от вершины source до всех остальных
    // возвращает вектор расстояний
    // если обнаружен отрицательный цикл, возвращает пустой вектор
    std::vector<T> findShortestPaths(int source);
    
    // найти кратчайший путь от source до target
    // возвращает расстояние (INF если пути нет)
    T findShortestPath(int source, int target);
    
    // найти кратчайший путь с восстановлением маршрута
    // возвращает пару: расстояние и вектор вершин пути
    std::pair<T, std::vector<int>> findShortestPathWithRoute(int source, int target);
    
    // проверить наличие отрицательного цикла, достижимого из source
    bool hasNegativeCycle(int source);
};

// реализация методов шаблонного класса

#include <algorithm>

// конструктор: сохраняем ссылку на граф
template <typename T>
BellmanFord<T>::BellmanFord(const Graph<T>& g) : graph(g), numVertices(g.getNumVertices()) {}

// найти кратчайшие пути от вершины source до всех остальных
template <typename T>
std::vector<T> BellmanFord<T>::findShortestPaths(int source) {
    // инициализация расстояний
    std::vector<T> dist(numVertices, INF);
    dist[source] = T(0);
    
    const auto& adjMatrix = graph.getAdjMatrix();
    
    // релаксация рёбер (numVertices - 1) раз
    for (int i = 0; i < numVertices - 1; i++) {
        bool updated = false;
        for (int u = 0; u < numVertices; u++) {
            if (dist[u] == INF) continue;
            for (int v = 0; v < numVertices; v++) {
                if (adjMatrix[u][v] != T(0)) {
                    T newDist = dist[u] + adjMatrix[u][v];
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        updated = true;
                    }
                }
            }
        }
        // если на итерации не было обновлений, можем прекратить
        if (!updated) break;
    }
    
    // проверка на отрицательные циклы
    for (int u = 0; u < numVertices; u++) {
        if (dist[u] == INF) continue;
        for (int v = 0; v < numVertices; v++) {
            if (adjMatrix[u][v] != T(0)) {
                T newDist = dist[u] + adjMatrix[u][v];
                if (newDist < dist[v]) {
                    // обнаружен отрицательный цикл
                    return std::vector<T>();
                }
            }
        }
    }
    
    return dist;
}

// найти кратчайший путь от source до target
template <typename T>
T BellmanFord<T>::findShortestPath(int source, int target) {
    auto dist = findShortestPaths(source);
    if (dist.empty()) return INF;  // есть отрицательный цикл
    return dist[target];
}

// найти кратчайший путь с восстановлением маршрута
template <typename T>
std::pair<T, std::vector<int>> BellmanFord<T>::findShortestPathWithRoute(int source, int target) {
    // инициализация
    std::vector<T> dist(numVertices, INF);
    std::vector<int> parent(numVertices, -1);
    dist[source] = T(0);
    
    const auto& adjMatrix = graph.getAdjMatrix();
    
    // релаксация рёбер
    for (int i = 0; i < numVertices - 1; i++) {
        bool updated = false;
        for (int u = 0; u < numVertices; u++) {
            if (dist[u] == INF) continue;
            for (int v = 0; v < numVertices; v++) {
                if (adjMatrix[u][v] != T(0)) {
                    T newDist = dist[u] + adjMatrix[u][v];
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
        }
        if (!updated) break;
    }
    
    // проверка на отрицательные циклы
    for (int u = 0; u < numVertices; u++) {
        if (dist[u] == INF) continue;
        for (int v = 0; v < numVertices; v++) {
            if (adjMatrix[u][v] != T(0)) {
                T newDist = dist[u] + adjMatrix[u][v];
                if (newDist < dist[v]) {
                    // отрицательный цикл обнаружен
                    return {INF, std::vector<int>()};
                }
            }
        }
    }
    
    // восстановление пути
    std::vector<int> path;
    if (dist[target] != INF) {
        int current = target;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
    }
    
    return {dist[target], path};
}

// проверить наличие отрицательного цикла
template <typename T>
bool BellmanFord<T>::hasNegativeCycle(int source) {
    auto dist = findShortestPaths(source);
    return dist.empty();
}

#endif // BELLMAN_FORD_H
