#include "../include/hamilton.h"
#include <iostream>
#include <algorithm>

// конструктор
template <typename T>
HamiltonGraph<T>::HamiltonGraph(Graph<T>* g) : graph(g), numVertices(g->getNumVertices()) {}

// проверить, можно ли добавить вершину v в позицию pos
template <typename T>
bool HamiltonGraph<T>::isSafe(int v, const std::vector<int>& path, int pos) const {
    const auto& adjMatrix = graph->getAdjMatrix();
    
    // проверить, есть ли ребро от предыдущей вершины к v
    if (adjMatrix[path[pos - 1]][v] == 0) {
        return false;
    }
    
    // проверить, что вершина v ещё не в пути
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return false;
        }
    }
    
    return true;
}

// вспомогательная функция для поиска гамильтонова цикла (backtracking)
template <typename T>
bool HamiltonGraph<T>::hamiltonianCycleUtil(std::vector<int>& path, std::vector<bool>& visited, int pos) {
    const auto& adjMatrix = graph->getAdjMatrix();
    
    // если все вершины добавлены в путь
    if (pos == numVertices) {
        // проверить, есть ли ребро от последней вершины к первой
        if (adjMatrix[path[pos - 1]][path[0]] != 0) {
            return true;
        }
        return false;
    }
    
    // пробуем добавить разные вершины
    for (int v = 1; v < numVertices; v++) {
        if (isSafe(v, path, pos)) {
            path[pos] = v;
            visited[v] = true;
            
            if (hamiltonianCycleUtil(path, visited, pos + 1)) {
                return true;
            }
            
            // backtrack
            visited[v] = false;
            path[pos] = -1;
        }
    }
    
    return false;
}

// найти гамильтонов цикл
template <typename T>
std::vector<int> HamiltonGraph<T>::findHamiltonianCycle() {
    std::vector<int> path(numVertices, -1);
    std::vector<bool> visited(numVertices, false);
    
    // начинаем с вершины 0
    path[0] = 0;
    visited[0] = true;
    
    if (hamiltonianCycleUtil(path, visited, 1)) {
        return path;
    }
    
    return std::vector<int>(); // пустой вектор, если цикл не найден
}

// проверить, является ли граф гамильтоновым
template <typename T>
bool HamiltonGraph<T>::isHamiltonian() {
    auto cycle = findHamiltonianCycle();
    return !cycle.empty();
}

// модифицировать граф до гамильтонова
template <typename T>
std::vector<std::pair<int, int>> HamiltonGraph<T>::makeHamiltonian() {
    std::vector<std::pair<int, int>> addedEdges;
    
    // если граф уже гамильтонов
    if (isHamiltonian()) {
        return addedEdges;
    }
    
    // простая эвристика: добавить рёбра чтобы сформировать цикл через все вершины
    // создаём простой цикл 0 -> 1 -> 2 -> ... -> n-1 -> 0
    const auto& adjMatrix = graph->getAdjMatrix();
    
    for (int i = 0; i < numVertices; i++) {
        int next = (i + 1) % numVertices;
        
        // если ребро не существует, добавляем его
        if (adjMatrix[i][next] == 0) {
            graph->addEdge(i, next, static_cast<T>(1));
            addedEdges.push_back({i, next});
        }
    }
    
    // также добавим несколько диагональных рёбер для большей связности
    // это гарантирует, что граф будет иметь много гамильтоновых циклов
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 2; j < numVertices; j++) {
            // не добавляем слишком много рёбер, только некоторые
            if ((i + j) % 3 == 0 && adjMatrix[i][j] == 0) {
                graph->addEdge(i, j, static_cast<T>(1));
                addedEdges.push_back({i, j});
            }
        }
    }
    
    return addedEdges;
}

// явная инстанциация шаблонов
template class HamiltonGraph<double>;
template class HamiltonGraph<int>;
