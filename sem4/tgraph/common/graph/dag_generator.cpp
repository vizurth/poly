#include "dag_generator.h"
#include <algorithm>
#include <cmath>
#include <stack>

using namespace std;

/*
    LOOK: DAGGenerator(WeibullDistribution& dist) 
    Конструктор класса seed генерируем автоматически
*/
template<typename T>
DAGGenerator<T>::DAGGenerator(WeibullDistribution& dist) 
    : distribution(dist), generator(random_device{}()) {}

/*
    LOOK: DAGGenerator(WeibullDistribution& dist, unsigned int seed) 
    Конструктор класса с заданным seed
*/
template<typename T>
DAGGenerator<T>::DAGGenerator(WeibullDistribution& dist, unsigned int seed) 
    : distribution(dist), generator(seed) {}

/*
    LOOK: isConnected(const Graph<T>&)
    Проверяем, что наш граф связный; проходимся по графу с помощью DFS и проверяем является число вершин в которые прошли общим количеством вершин
*/
template<typename T>
bool DAGGenerator<T>::isConnected(const Graph<T>& graph) {
    int n = graph.getNumVertices();
    if (n == 0) return true;
    
    vector<bool> visited(n, false);
    stack<int> s;
    
    // начинаем обход с вершины 0
    s.push(0);
    visited[0] = true;
    int visitedCount = 1;
    
    while (!s.empty()) {
        int u = s.top();
        s.pop();
        
        // проверяем все исходящие рёбра
        auto outgoing = graph.getOutgoingEdges(u);
        for (const auto& edge : outgoing) {
            int v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                visitedCount++;
                s.push(v);
            }
        }
        
        // про��еряем все входящие рёбра (для связности)
        auto incoming = graph.getIncomingEdges(u);
        for (const auto& edge : incoming) {
            int v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                visitedCount++;
                s.push(v);
            }
        }
    }
    
    return visitedCount == n;
}

/*
    LOOK: Graph<T> generateDAG(int numVertices, bool isDirected, WeightType weightType)
    Генерируем связный граф с заданным количеством вершин
    Веса генерируются по распределению Вейбулла
    isDirected = true: ориентированный ациклический граф (DAG)
    isDirected = false: неориентированный граф (добавляем обратные рёбра)
    weightType: тип весов (положительные/отрицательные/смешанные)
*/
template<typename T>
Graph<T> DAGGenerator<T>::generateDAG(int numVertices, bool isDirected, WeightType weightType) {
    Graph<T> graph(numVertices);
    
    if (numVertices <= 0) {
        return graph;
    }
    
    uniform_real_distribution<double> signDist(0.0, 1.0);
    
    auto generateWeight = [&]() -> T {
        double weight = distribution.generate(); // Вейбулл всегда генерирует положительные числа
        if (weight < 0.1) weight = 0.1; // минимальный вес
        
        // применяем знак в зависимости от типа весов
        switch (weightType) {
            case WeightType::POSITIVE:
                return static_cast<T>(weight);
            case WeightType::NEGATIVE:
                return static_cast<T>(-weight);
            case WeightType::MIXED:
                // 50% вероятность отрицательного веса
                return static_cast<T>((signDist(generator) < 0.5) ? weight : -weight);
        }
        return static_cast<T>(weight);
    };
    
    // создаём базовый связный путь: 0 -> 1 -> 2 -> ... -> n-1
    for (int i = 0; i < numVertices - 1; i++) {
        T weight = generateWeight();
        graph.addEdge(i, i + 1, weight);
        
        // если неориентированный, добавляем обратное ребро с тем же весом
        if (!isDirected) {
            graph.addEdge(i + 1, i, weight);
        }
    }
    
    // добавляем случайные рёбра
    double probability = min(0.5, 3.0 / numVertices);
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    if (isDirected) {
        // для ориентированного: только вперёд (i -> j, где i < j)
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 2; j < numVertices; j++) {
                if (dist(generator) < probability) {
                    T weight = generateWeight();
                    graph.addEdge(i, j, weight);
                }
            }
        }
    } else {
        // для неориентированного: в обе стороны
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 2; j < numVertices; j++) {
                if (dist(generator) < probability) {
                    T weight = generateWeight();
                    graph.addEdge(i, j, weight);
                    graph.addEdge(j, i, weight); // обратное ребро
                }
            }
        }
    }
    
    return graph;
}