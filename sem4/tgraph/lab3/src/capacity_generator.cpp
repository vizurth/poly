#include "../include/capacity_generator.h"
#include <cmath>

// конструктор: сохраняем указатель на распределение
CapacityGenerator::CapacityGenerator(Distribution* dist) : distribution(dist) {}

// генерация положительного целого числа из распределения
int CapacityGenerator::generatePositiveInt() {
    double value = distribution->generate();
    // берём модуль и округляем до целого
    int result = static_cast<int>(std::abs(value));
    // если получился 0, возвращаем 1
    return (result == 0) ? 1 : result;
}

// сгенерировать матрицы пропускных способностей и стоимостей
std::pair<Graph<int>, Graph<double>> CapacityGenerator::generateMatrices(const Graph<double>& graph) {
    int n = graph.getNumVertices();
    const auto& adjMatrix = graph.getAdjMatrix();
    
    // создаём пустые графы для пропускных способностей и стоимостей
    Graph<int> capacity(n);
    Graph<double> cost(n);
    
    // для каждого ребра в исходном графе генерируем capacity и cost
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] > 0.0) {
                // ребро существует - генерируем пропускную способность и стоимость
                int cap = generatePositiveInt();
                capacity.addEdge(i, j, cap);
                
                // генерируем стоимость (вещественное число, берём из распределения)
                double c = distribution->generate();
                if (c <= 0) {
                    c = 1.0; // гарантируем положительную стоимость
                }
                cost.addEdge(i, j, c);
            }
        }
    }
    
    return std::make_pair(capacity, cost);
}
