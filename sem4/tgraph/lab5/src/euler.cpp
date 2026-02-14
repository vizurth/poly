#include "../include/euler.h"
#include <iostream>
#include <stack>
#include <algorithm>
#include <queue>

// конструктор
template <typename T>
EulerGraph<T>::EulerGraph(Graph<T>* g) : graph(g), numVertices(g->getNumVertices()) {}

// проверить связность графа (обход в глубину)
template <typename T>
bool EulerGraph<T>::isConnected() const {
    std::vector<bool> visited(numVertices, false);
    const auto& adjMatrix = graph->getAdjMatrix();
    
    // найти первую вершину с ненулевой степенью
    int startVertex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (getDegree(i) > 0) {
            startVertex = i;
            break;
        }
    }
    
    if (startVertex == -1) {
        return true; // нет рёбер, граф связный
    }
    
    // обход в глубину
    std::stack<int> stack;
    stack.push(startVertex);
    visited[startVertex] = true;
    
    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();
        
        for (int u = 0; u < numVertices; u++) {
            // для неориентированного графа проверяем оба направления
            if (!visited[u] && (adjMatrix[v][u] != 0 || adjMatrix[u][v] != 0)) {
                visited[u] = true;
                stack.push(u);
            }
        }
    }
    
    // проверить, что все вершины с ненулевой степенью посещены
    for (int i = 0; i < numVertices; i++) {
        if (getDegree(i) > 0 && !visited[i]) {
            return false;
        }
    }
    
    return true;
}

// подсчитать степень вершины (для неориентированного графа)
template <typename T>
int EulerGraph<T>::getDegree(int vertex) const {
    const auto& adjMatrix = graph->getAdjMatrix();
    int degree = 0;
    
    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[vertex][i] != 0) {
            degree++;
        }
        // для неориентированного графа считаем входящие рёбра
        if (i != vertex && adjMatrix[i][vertex] != 0) {
            degree++;
        }
    }
    
    return degree;
}

// получить список вершин с нечётной степенью
template <typename T>
std::vector<int> EulerGraph<T>::getOddDegreeVertices() const {
    std::vector<int> oddVertices;
    
    for (int i = 0; i < numVertices; i++) {
        if (getDegree(i) % 2 == 1) {
            oddVertices.push_back(i);
        }
    }
    
    return oddVertices;
}

// проверить, является ли граф эйлеровым (цикл)
template <typename T>
bool EulerGraph<T>::isEulerian() const {
    // эйлеров цикл существует, если:
    // 1. граф связный
    // 2. все вершины имеют чётную степень
    
    if (!isConnected()) {
        return false;
    }
    
    auto oddVertices = getOddDegreeVertices();
    return oddVertices.empty();
}

// проверить, существует ли эйлеров путь (не цикл)
template <typename T>
bool EulerGraph<T>::hasEulerianPath() const {
    // эйлеров путь существует, если:
    // 1. граф связный
    // 2. ровно 2 вершины с нечётной степенью
    
    if (!isConnected()) {
        return false;
    }
    
    auto oddVertices = getOddDegreeVertices();
    return oddVertices.size() == 2;
}

// модифицировать граф до эйлерова
template <typename T>
std::vector<std::pair<int, int>> EulerGraph<T>::makeEulerian() {
    std::vector<std::pair<int, int>> addedEdges;
    
    // получить вершины с нечётной степенью
    auto oddVertices = getOddDegreeVertices();
    
    // если граф уже эйлеров
    if (oddVertices.empty()) {
        return addedEdges;
    }
    
    // соединить вершины с нечётной степенью попарно
    // это минимальный способ сделать все степени чётными
    for (size_t i = 0; i + 1 < oddVertices.size(); i += 2) {
        int v1 = oddVertices[i];
        int v2 = oddVertices[i + 1];
        
        // добавить ребро с весом 1 (минимальный вес)
        graph->addEdge(v1, v2, static_cast<T>(1));
        // для неориентированного графа добавляем обратное ребро
        graph->addEdge(v2, v1, static_cast<T>(1));
        
        addedEdges.push_back({v1, v2});
    }
    
    return addedEdges;
}

// построить эйлеров цикл алгоритмом хирхольцера
template <typename T>
void EulerGraph<T>::hierholzerDFS(int vertex, std::vector<std::vector<int>>& adjList, std::vector<int>& circuit) {
    while (!adjList[vertex].empty()) {
        int next = adjList[vertex].back();
        adjList[vertex].pop_back();
        hierholzerDFS(next, adjList, circuit);
    }
    circuit.push_back(vertex);
}

// построить эйлеров цикл
template <typename T>
std::vector<int> EulerGraph<T>::findEulerianCircuit() {
    std::vector<int> circuit;
    
    if (!isEulerian()) {
        std::cout << "граф не является эйлеровым, невозможно построить цикл\n";
        return circuit;
    }
    
    // создать список смежности (копия для модификации)
    std::vector<std::vector<int>> adjList(numVertices);
    const auto& adjMatrix = graph->getAdjMatrix();
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] != 0) {
                adjList[i].push_back(j);
            }
        }
    }
    
    // найти стартовую вершину (любую с ненулевой степенью)
    int startVertex = 0;
    for (int i = 0; i < numVertices; i++) {
        if (!adjList[i].empty()) {
            startVertex = i;
            break;
        }
    }
    
    // построить цикл алгоритмом хирхольцера
    hierholzerDFS(startVertex, adjList, circuit);
    
    // перевернуть, так как dfs добавляет в обратном порядке
    std::reverse(circuit.begin(), circuit.end());
    
    return circuit;
}

// вывести информацию о степенях вершин
template <typename T>
void EulerGraph<T>::printDegrees() const {
    std::cout << "\nстепени вершин:\n";
    
    for (int i = 0; i < numVertices; i++) {
        int degree = getDegree(i);
        std::cout << "вершина " << i << ": степень " << degree;
        if (degree % 2 == 1) {
            std::cout << " (нечётная)";
        } else {
            std::cout << " (чётная)";
        }
        std::cout << "\n";
    }
}

// явная инстанциация шаблонов
template class EulerGraph<double>;
template class EulerGraph<int>;
