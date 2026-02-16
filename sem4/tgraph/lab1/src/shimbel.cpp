#include "lab1/include/shimbel.h"

template <typename T>
ShimbelSolver<T>::ShimbelSolver(const Graph<T>& g) : graph(g), n(g.getNumVertices()) {
    currentMatrix.resize(n, std::vector<T>(n, getINF()));
}

template <typename T>
void ShimbelSolver<T>::initializeMatrix() {
    // заполняем матрицу весами из графа
    // для несуществующих рёбер ставим INF
    T INF = getINF();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                currentMatrix[i][j] = 0; // нет петель в DAG
            } else if (graph.hasEdge(i, j)) {
                currentMatrix[i][j] = graph.getEdge(i, j);
            } else {
                currentMatrix[i][j] = INF;
            }
        }
    }
}

template <typename T>
void ShimbelSolver<T>::multiplyMatrix(bool findMin) {
    // создаём новую матрицу для D^(k)
    T INF = getINF();
    std::vector<std::vector<T>> newMatrix(n, std::vector<T>(n, INF));
    
    // D^(k)[i][j] = min/max по всем p: D^(k-1)[i][p] + A[p][j]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::vector<T> possiblePaths; // все возможные пути
            
            for (int p = 0; p < n; p++) {
                // проверяем есть ли путь: i -> p (k-1 рёбер) и p -> j (1 ребро)
                if (currentMatrix[i][p] != INF && currentMatrix[i][p] != 0 && graph.hasEdge(p, j)) {
                    T pathWeight = currentMatrix[i][p] + graph.getEdge(p, j);
                    possiblePaths.push_back(pathWeight);
                }
            }
            
            // если нет путей, ставим INF
            if (possiblePaths.empty()) {
                newMatrix[i][j] = INF;
            } else {
                // выбираем минимум или максимум из всех путей
                if (findMin) {
                    newMatrix[i][j] = *std::min_element(possiblePaths.begin(), possiblePaths.end());
                } else {
                    newMatrix[i][j] = *std::max_element(possiblePaths.begin(), possiblePaths.end());
                }
            }
        }
    }
    
    currentMatrix = newMatrix;
}

template <typename T>
std::vector<std::vector<T>> ShimbelSolver<T>::computeShortestPaths(int K) {
    if (K <= 0) {
        return std::vector<std::vector<T>>(n, std::vector<T>(n, getINF()));
    }
    
    initializeMatrix();
    
    // D^(1) уже есть (пути через 1 ребро)
    // для K=2 делаем 1 умножение, для K=3 делаем 2 умножения и т.д.
    for (int k = 2; k <= K; k++) {
        multiplyMatrix(true); // ищем минимум
    }
    
    return currentMatrix;
}

template <typename T>
std::vector<std::vector<T>> ShimbelSolver<T>::computeLongestPaths(int K) {
    if (K <= 0) {
        return std::vector<std::vector<T>>(n, std::vector<T>(n, getINF()));
    }
    
    initializeMatrix();
    
    // D^(1) уже есть (пути через 1 ребро)
    // для K=2 делаем 1 умножение, для K=3 делаем 2 умножения и т.д.
    for (int k = 2; k <= K; k++) {
        multiplyMatrix(false); // ищем максимум
    }
    
    return currentMatrix;
}

template <typename T>
void ShimbelSolver<T>::printMatrix(const std::vector<std::vector<T>>& matrix, const std::string& title) {
    int n = matrix.size();
    T INF = getINF();
    
    std::cout << "\n" << title << "\n";
    
    // шапка таблицы
    std::cout << "    ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(8) << i;
    }
    std::cout << "\n";
    
    // строки матрицы
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != INF) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j];
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}