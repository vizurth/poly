#ifndef DAG_GENERATOR_H
#define DAG_GENERATOR_H

#include "graph.h"
#include "../distributions/distribution.h"

// генератор связного ациклического направленного графа (DAG)
// T - тип весов рёбер (обычно double)
template <typename T = double>
class DAGGenerator {
private:
    Distribution* distribution;  // распределение для генерации весов рёбер
    
public:
    // конструктор: принимает распределение для генерации весов
    DAGGenerator(Distribution* dist);
    
    // сгенерировать связный ациклический граф с заданным количеством вершин
    // алгоритм:
    // 1. создаём топологический порядок вершин (0, 1, 2, ..., n-1)
    // 2. добавляем рёбра только от меньших номеров к большим (гарантия ацикличности)
    // 3. гарантируем связность: каждая вершина i > 0 имеет хотя бы одно входящее ребро
    Graph<T> generateDAG(int numVertices);
    
private:
    // генерация положительного веса ребра (если получили отрицательное, берём модуль)
    T generatePositiveWeight();
};

// реализация методов шаблонного класса

#include <cmath>
#include <random>
#include <chrono>

// конструктор: сохраняем указатель на распределение
template <typename T>
DAGGenerator<T>::DAGGenerator(Distribution* dist) : distribution(dist) {}

// генерация положительного веса ребра
// если получили отрицательное значение, берём его модуль
template <typename T>
T DAGGenerator<T>::generatePositiveWeight() {
    double weight = distribution->generate();
    return static_cast<T>(std::abs(weight));
}

// сгенерировать связный ациклический граф
template <typename T>
Graph<T> DAGGenerator<T>::generateDAG(int numVertices) {
    Graph<T> graph(numVertices);
    
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
        T weight = generatePositiveWeight();
        graph.addEdge(j, i, weight);
    }
    
    // добавляем дополнительные рёбра для увеличения плотности графа
    // проходим по всем парам вершин (i, j) где i < j
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            // с вероятностью 30% добавляем ребро i -> j
            std::uniform_real_distribution<double> prob(0.0, 1.0);
            if (prob(rng) < 0.3) {
                T weight = generatePositiveWeight();
                graph.addEdge(i, j, weight);
            }
        }
    }
    
    return graph;
}

#endif // DAG_GENERATOR_H
