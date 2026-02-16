#include "dag_generator.h"
#include <algorithm>
#include <cmath>
#include <stack>

using namespace std;

// конструктор: принимает распределение для генерации весов и зерно для генератора случайных чисел
template<typename T>
DAGGenerator<T>::DAGGenerator(NormalDistribution& dist, unsigned int seed) 
    : distribution(dist), generator(seed) {}

// проверить, что граф связный (используя DFS)
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
        
        // проверяем все входящие рёбра (для связности)
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

// сгенерировать связный ациклический граф с заданным количеством вершин
template<typename T>
Graph<T> DAGGenerator<T>::generateDAG(int numVertices) {
    Graph<T> graph(numVertices);
    
    if (numVertices <= 0) {
        return graph;
    }
    
    // для DAG: вершины упорядочены, рёбра идут только от меньших номеров к большим
    // это автоматически гарантирует ацикличность
    
    // сначала создаём базовый связный путь: 0 -> 1 -> 2 -> ... -> n-1
    // это гарантирует связность
    for (int i = 0; i < numVertices - 1; i++) {
        double weight = abs(distribution.generate());
        if (weight == 0) weight = 1.0; // избегаем нулевых весов
        graph.addEdge(i, i + 1, static_cast<T>(weight));
    }
    
    // теперь добавляем случайные рёбра с вероятностью p
    // вероятность зависит от размера графа (чтобы не было слишком много рёбер)
    double probability = min(0.5, 3.0 / numVertices);
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 2; j < numVertices; j++) { // i+2 чтобы не дублировать базовый путь
            // добавляем ребро с вероятностью p
            if (dist(generator) < probability) {
                double weight = abs(distribution.generate());
                if (weight == 0) weight = 1.0;
                graph.addEdge(i, j, static_cast<T>(weight));
            }
        }
    }
    
    return graph;
}

// явная инстанциация шаблонов
template class DAGGenerator<double>;
template class DAGGenerator<int>;
template class DAGGenerator<float>;