#ifndef SHIMBEL_H
#define SHIMBEL_H

#include "common/graph/graph.h"
#include <vector>
#include <limits>
#include <iomanip>
#include <iostream>

using namespace std;

template <typename T>
class ShimbelSolver {
private:
    const Graph<T>& graph;
    int n; // number of vertices
    std::vector<std::vector<T>> currentMatrix;
    
    static T getINF() {
        return std::numeric_limits<T>::max() / 2;
    }
    
    void initializeMatrix();
    void multiplyMatrix(bool findMin);
    
public:
    explicit ShimbelSolver(const Graph<T>& g);
    
	// кратчайшие пути с точно K рёбрами
    std::vector<std::vector<T>> computeShortestPaths(int K);
    
    // самые длинные пути с точно K рёбрами
    std::vector<std::vector<T>> computeLongestPaths(int K);
    
    // утилита для печати матрицы
    static void printMatrix(const std::vector<std::vector<T>>& matrix, const std::string& title);
};

#include "lab1/src/shimbel.cpp"

#endif // SHIMBEL_H
