#include "dag_generator.h"
#include <cmath>
#include <random>
#include <chrono>

// конструктор: сохраняем указатель на распределение
DAGGenerator::DAGGenerator(Distribution* dist) : distribution(dist) {}

// генерация положительного веса ребра
// если получили отрицательное значение, берём его модуль
double DAGGenerator::generatePositiveWeight() {
    double weight = distribution->generate();
    return std::abs(weight);
}

// сгенерировать связный ациклический граф
Graph DAGGenerator::generateDAG(int numVertices) {
    Graph graph(numVertices);
    
    // инициализируем генератор случайных чисел
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    
    // для каждой вершины i (начиная с 1) создаём хотя бы одно входящее ребро
    // это гарантирует связность графа
    for (int i = 1; i < numVertices; i++) {
        // выбираем случайную вершину j с номером меньше i
        std::uniform_int_distribution<int> dist(0, i - 1);
        int j = dist(rng);
        
        // добавляем ребро j -> i с положительным весом
        double weight = generatePositiveWeight();
        graph.addEdge(j, i, weight);
    }
    
    // добавляем дополнительные рёбра для увеличения плотности графа
    // проходим по всем парам вершин (i, j) где i < j
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            // с вероятностью 30% добавляем ребро i -> j
            std::uniform_real_distribution<double> prob(0.0, 1.0);
            if (prob(rng) < 0.3) {
                double weight = generatePositiveWeight();
                graph.addEdge(i, j, weight);
            }
        }
    }
    
    return graph;
}
