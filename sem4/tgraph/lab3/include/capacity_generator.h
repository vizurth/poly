#ifndef CAPACITY_GENERATOR_H
#define CAPACITY_GENERATOR_H

#include "../../common/graph/graph.h"
#include "../../common/distributions/distribution.h"
#include <utility>

// генератор матриц пропускных способностей и стоимостей для потоковых сетей
class CapacityGenerator {
private:
    Distribution* distribution;
    
public:
    // конструктор: принимает распределение для генерации значений
    CapacityGenerator(Distribution* dist);
    
    // сгенерировать матрицы пропускных способностей и стоимостей на основе графа
    // возвращает пару: (capacity, cost)
    // capacity - максимальный поток через ребро (положительное целое)
    // cost - стоимость единицы потока через ребро (положительное вещественное)
    std::pair<Graph<int>, Graph<double>> generateMatrices(const Graph<double>& graph);
    
private:
    // генерация положительного целого числа из распределения
    int generatePositiveInt();
};

#endif // CAPACITY_GENERATOR_H
