#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <iomanip>

// шаблонный класс графа с матрицей смежности
// T - тип весов рёбер (double для взвешенных графов, int для сетей и т.д.)
template <typename T>
class Graph {
protected:
    int numVertices;  // количество вершин
    std::vector<std::vector<T>> adjMatrix;  // матрица смежности (веса рёбер)
    
public:
    // конструктор: создаёт граф с заданным количеством вершин
    Graph(int vertices);
    
    // добавить ребро от вершины from к вершине to с весом weight
    void addEdge(int from, int to, T weight);
    
    // получить вес ребра между вершинами from и to
    T getEdgeWeight(int from, int to) const;
    
    // проверить, существует ли ребро между вершинами
    bool hasEdge(int from, int to) const;
    
    // получить количество вершин
    int getNumVertices() const;
    
    // получить матрицу смежности
    const std::vector<std::vector<T>>& getAdjMatrix() const;
    
    // вывести матрицу смежности на экран
    void printAdjMatrix() const;
    
    // вывести список рёбер
    void printEdges() const;
};

// реализация методов шаблонного класса (должна быть в заголовочном файле)

// конструктор: создаём граф с заданным количеством вершин
// инициализируем матрицу смежности нулями (нет рёбер)
template <typename T>
Graph<T>::Graph(int vertices) : numVertices(vertices) {
    adjMatrix.resize(vertices, std::vector<T>(vertices, T(0)));
}

// добавить ребро от вершины from к вершине to с весом weight
template <typename T>
void Graph<T>::addEdge(int from, int to, T weight) {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix[from][to] = weight;
    }
}

// получить вес ребра между вершинами from и to
template <typename T>
T Graph<T>::getEdgeWeight(int from, int to) const {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        return adjMatrix[from][to];
    }
    return T(0);
}

// проверить, существует ли ребро между вершинами
// ребро существует, если его вес не равен нулю
template <typename T>
bool Graph<T>::hasEdge(int from, int to) const {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        return adjMatrix[from][to] != T(0);
    }
    return false;
}

// получить количество вершин в графе
template <typename T>
int Graph<T>::getNumVertices() const {
    return numVertices;
}

// получить матрицу смежности
template <typename T>
const std::vector<std::vector<T>>& Graph<T>::getAdjMatrix() const {
    return adjMatrix;
}

// вывести матрицу смежности на экран
template <typename T>
void Graph<T>::printAdjMatrix() const {
    std::cout << "\nматрица смежности (веса рёбер):\n";
    std::cout << "    ";
    
    // вывести номера столбцов
    for (int i = 0; i < numVertices; i++) {
        std::cout << std::setw(8) << i;
    }
    std::cout << "\n";
    
    // вывести строки матрицы
    for (int i = 0; i < numVertices; i++) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] != T(0)) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(2) << adjMatrix[i][j];
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << "\n";
    }
}

// вывести список рёбер с их весами
template <typename T>
void Graph<T>::printEdges() const {
    std::cout << "\nсписок рёбер:\n";
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] != T(0)) {
                std::cout << "ребро " << i << " -> " << j 
                         << ", вес: " << std::fixed << std::setprecision(2) 
                         << adjMatrix[i][j] << "\n";
            }
        }
    }
}

#endif // GRAPH_H
