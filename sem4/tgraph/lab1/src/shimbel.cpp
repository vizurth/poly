#include "../include/shimbel.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// конструктор: сохраняем ссылку на граф
template <typename T>
Shimbel<T>::Shimbel(const Graph<T>& g) : graph(g), numVertices(g.getNumVertices()) {}

// метод Шимбелла для поиска минимальных путей длины ровно k рёбер
// используем динамическое программирование: D[k][i][j] - минимальный путь из i в j длины k
template <typename T>
std::vector<std::vector<T>> Shimbel<T>::findMinPaths(int k) {
    // инициализация: D[0] - нулевая матрица (путь длины 0 от вершины к себе)
    std::vector<std::vector<T>> prev(numVertices, std::vector<T>(numVertices, INF));
    for (int i = 0; i < numVertices; i++) {
        prev[i][i] = T(0);
    }
    
    // D[1] - матрица смежности (пути длины 1)
    const auto& adjMatrix = graph.getAdjMatrix();
    std::vector<std::vector<T>> current(numVertices, std::vector<T>(numVertices, INF));
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] != T(0)) {
                current[i][j] = adjMatrix[i][j];
            }
        }
    }
    
    // итеративно вычисляем D[m] для m от 2 до k
    // D[m][i][j] = min по всем вершинам v (D[m-1][i][v] + D[1][v][j])
    for (int m = 2; m <= k; m++) {
        std::vector<std::vector<T>> next(numVertices, std::vector<T>(numVertices, INF));
        
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                for (int v = 0; v < numVertices; v++) {
                    if (current[i][v] != INF && adjMatrix[v][j] != T(0)) {
                        next[i][j] = std::min(next[i][j], current[i][v] + adjMatrix[v][j]);
                    }
                }
            }
        }
        
        current = next;
    }
    
    return current;
}

// метод Шимбелла для поиска максимальных путей длины ровно k рёбер
// аналогично минимальным путям, но используем операцию max вместо min
template <typename T>
std::vector<std::vector<T>> Shimbel<T>::findMaxPaths(int k) {
    // инициализация: D[0] - нулевая матрица
    T negInf = -std::numeric_limits<T>::infinity();
    std::vector<std::vector<T>> prev(numVertices, std::vector<T>(numVertices, negInf));
    for (int i = 0; i < numVertices; i++) {
        prev[i][i] = T(0);
    }
    
    // D[1] - матрица смежности
    const auto& adjMatrix = graph.getAdjMatrix();
    std::vector<std::vector<T>> current(numVertices, std::vector<T>(numVertices, negInf));
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] != T(0)) {
                current[i][j] = adjMatrix[i][j];
            }
        }
    }
    
    // итеративно вычисляем D[m] для m от 2 до k
    // D[m][i][j] = max по всем вершинам v (D[m-1][i][v] + D[1][v][j])
    for (int m = 2; m <= k; m++) {
        std::vector<std::vector<T>> next(numVertices, std::vector<T>(numVertices, negInf));
        
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                for (int v = 0; v < numVertices; v++) {
                    if (current[i][v] != negInf && adjMatrix[v][j] != T(0)) {
                        next[i][j] = std::max(next[i][j], current[i][v] + adjMatrix[v][j]);
                    }
                }
            }
        }
        
        current = next;
    }
    
    return current;
}
// вывести матрицу путей на экран
template <typename T>
void Shimbel<T>::printPathMatrix(const std::vector<std::vector<T>>& matrix, 
                              const std::string& title) {
    std::cout << "\n" << title << ":\n";
    int n = matrix.size();
    
    // вывести номера столбцов
    std::cout << "    ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(8) << i;
    }
    std::cout << "\n";
    
    // вывести строки матрицы
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < n; j++) {
            if (std::isinf(matrix[i][j]) || matrix[i][j] == -std::numeric_limits<T>::infinity()) {
                std::cout << std::setw(10) << "∞";
            } else {
                std::cout << std::setw(10) << std::fixed << std::setprecision(2) << matrix[i][j];
            }
        }
        std::cout << "\n";
    }
}

// явная инстанциация шаблона для double
template class Shimbel<double>;
