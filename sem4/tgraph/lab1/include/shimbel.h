#ifndef SHIMBEL_H
#define SHIMBEL_H

#include "../../common/graph/graph.h"
#include <vector>
#include <limits>

// класс для работы с методом Шимбелла
class Shimbel {
private:
    const Graph& graph;
    int numVertices;
    const double INF = std::numeric_limits<double>::infinity();
    
public:
    // конструктор: принимает граф для анализа
    Shimbel(const Graph& g);
    
    // найти минимальные пути длины ровно k рёбер
    // возвращает матрицу минимальных весов путей
    std::vector<std::vector<double>> findMinPaths(int k);
    
    // найти максимальные пути длины ровно k рёбер
    // возвращает матрицу максимальных весов путей
    std::vector<std::vector<double>> findMaxPaths(int k);
    
    // вывести матрицу путей на экран
    static void printPathMatrix(const std::vector<std::vector<double>>& matrix, 
                                const std::string& title);
};

#endif // SHIMBEL_H
