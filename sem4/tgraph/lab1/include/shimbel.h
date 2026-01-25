#ifndef SHIMBEL_H
#define SHIMBEL_H

#include "../../common/graph/graph.h"
#include <vector>
#include <limits>
#include <string>

// класс для работы с методом Шимбелла
// T - тип весов рёбер
template <typename T = double>
class Shimbel {
private:
    const Graph<T>& graph;
    int numVertices;
    const T INF = std::numeric_limits<T>::infinity();
    
public:
    // конструктор: принимает граф для анализа
    Shimbel(const Graph<T>& g);
    
    // найти минимальные пути длины ровно k рёбер
    // возвращает матрицу минимальных весов путей
    std::vector<std::vector<T>> findMinPaths(int k);
    
    // найти максимальные пути длины ровно k рёбер
    // возвращает матрицу максимальных весов путей
    std::vector<std::vector<T>> findMaxPaths(int k);
    
    // вывести матрицу путей на экран
    static void printPathMatrix(const std::vector<std::vector<T>>& matrix, 
                                const std::string& title);
};

#endif // SHIMBEL_H
