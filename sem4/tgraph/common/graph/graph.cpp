#include "graph.h"
#include <iomanip>
#include <cmath>

// конструктор: создаём граф с заданным количеством вершин
// инициализируем матрицу смежности нулями (нет рёбер)
Graph::Graph(int vertices) : numVertices(vertices) {
    adjMatrix.resize(vertices, std::vector<double>(vertices, 0.0));
}

// добавить ребро от вершины from к вершине to с весом weight
void Graph::addEdge(int from, int to, double weight) {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        adjMatrix[from][to] = weight;
    }
}

// получить вес ребра между вершинами from и to
double Graph::getEdgeWeight(int from, int to) const {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        return adjMatrix[from][to];
    }
    return 0.0;
}

// проверить, существует ли ребро между вершинами
// ребро существует, если его вес больше нуля
bool Graph::hasEdge(int from, int to) const {
    if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
        return adjMatrix[from][to] > 0.0;
    }
    return false;
}

// получить количество вершин в графе
int Graph::getNumVertices() const {
    return numVertices;
}

// получить матрицу смежности
const std::vector<std::vector<double>>& Graph::getAdjMatrix() const {
    return adjMatrix;
}

// вывести матрицу смежности на экран
void Graph::printAdjMatrix() const {
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
            if (adjMatrix[i][j] > 0.0) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(2) << adjMatrix[i][j];
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << "\n";
    }
}

// вывести список рёбер с их весами
void Graph::printEdges() const {
    std::cout << "\nсписок рёбер:\n";
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (adjMatrix[i][j] > 0.0) {
                std::cout << "ребро " << i << " -> " << j 
                         << ", вес: " << std::fixed << std::setprecision(2) 
                         << adjMatrix[i][j] << "\n";
            }
        }
    }
}
