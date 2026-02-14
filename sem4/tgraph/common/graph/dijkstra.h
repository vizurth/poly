#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <vector>
#include <limits>

// алгоритм Дейкстры для поиска кратчайших путей
// работает только с неотрицательными весами рёбер
template <typename T = double>
class Dijkstra {
private:
    const Graph<T>& graph;
    int numVertices;
    const T INF = std::numeric_limits<T>::max();
    
public:
    // конструктор: принимает граф для анализа
    Dijkstra(const Graph<T>& g);
    
    // найти кратчайшие пути от вершины source до всех остальных
    // возвращает вектор расстояний
    std::vector<T> findShortestPaths(int source);
    
    // найти кратчайший путь от source до target
    // возвращает расстояние (INF если пути нет)
    T findShortestPath(int source, int target);
    
    // найти кратчайший путь от source до target с восстановлением пути
    // возвращает пару: расстояние и вектор вершин пути
    std::pair<T, std::vector<int>> findShortestPathWithRoute(int source, int target);
};

// реализация методов шаблонного класса

#include <queue>
#include <algorithm>

// конструктор: сохраняем ссылку на граф
template <typename T>
Dijkstra<T>::Dijkstra(const Graph<T>& g) : graph(g), numVertices(g.getNumVertices()) {}

// найти кратчайшие пути от вершины source до всех остальных
template <typename T>
std::vector<T> Dijkstra<T>::findShortestPaths(int source) {
    // инициализация расстояний (все бесконечность кроме source)
    std::vector<T> dist(numVertices, INF);
    dist[source] = T(0);
    
    // массив посещённых вершин
    std::vector<bool> visited(numVertices, false);
    
    // приоритетная очередь: (расстояние, вершина)
    using Pair = std::pair<T, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({T(0), source});
    
    const auto& adjMatrix = graph.getAdjMatrix();
    
    while (!pq.empty()) {
        int u = pq.top().second;
        T currentDist = pq.top().first;
        pq.pop();
        
        // если уже посетили эту вершину, пропускаем
        if (visited[u]) continue;
        visited[u] = true;
        
        // проверяем все соседние вершины
        for (int v = 0; v < numVertices; v++) {
            if (adjMatrix[u][v] != T(0) && !visited[v]) {
                T newDist = dist[u] + adjMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    pq.push({newDist, v});
                }
            }
        }
    }
    
    return dist;
}

// найти кратчайший путь от source до target
template <typename T>
T Dijkstra<T>::findShortestPath(int source, int target) {
    auto dist = findShortestPaths(source);
    return dist[target];
}

// найти кратчайший путь с восстановлением маршрута
template <typename T>
std::pair<T, std::vector<int>> Dijkstra<T>::findShortestPathWithRoute(int source, int target) {
    // инициализация
    std::vector<T> dist(numVertices, INF);
    std::vector<int> parent(numVertices, -1);
    dist[source] = T(0);
    
    std::vector<bool> visited(numVertices, false);
    
    using Pair = std::pair<T, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({T(0), source});
    
    const auto& adjMatrix = graph.getAdjMatrix();
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // если достигли целевой вершины, можем прекратить поиск
        if (u == target) break;
        
        for (int v = 0; v < numVertices; v++) {
            if (adjMatrix[u][v] != T(0) && !visited[v]) {
                T newDist = dist[u] + adjMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    parent[v] = u;
                    pq.push({newDist, v});
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

#endif // DIJKSTRA_H
