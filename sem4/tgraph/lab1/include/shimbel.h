#pragma once

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
    int n; // количество вершин
    vector<vector<T>> currentMatrix; // текушая матрица
    
    static T getINF() {
        return numeric_limits<T>::max() / 2;
    }
    
    void initializeMatrix();
    void multiplyMatrix(bool findMin);
    
public:
    ShimbelSolver(const Graph<T>& g);
    
    vector<vector<T>> computeShortestPaths(int K);
    vector<vector<T>> computeLongestPaths(int K);
    
    static void printMatrix(const vector<vector<T>>& matrix, const string& title);
};

#include "lab1/src/shimbel.cpp"