#ifndef DAG_GENERATOR_H
#define DAG_GENERATOR_H

#include "graph.h"
#include "../distributions/distribution.h"

// генератор связного ациклического направленного графа (DAG)
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
    Graph generateDAG(int numVertices);
    
private:
    // генерация положительного веса ребра (если получили отрицательное, берём модуль)
    double generatePositiveWeight();
};

#endif // DAG_GENERATOR_H
