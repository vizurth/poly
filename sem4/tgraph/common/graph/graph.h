#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

// базовый класс графа с матрицей смежности
class Graph {
protected:
    int numVertices;  // количество вершин
    std::vector<std::vector<double>> adjMatrix;  // матрица смежности (веса рёбер)
    
public:
    // конструктор: создаёт граф с заданным количеством вершин
    Graph(int vertices);
    
    // добавить ребро от вершины from к вершине to с весом weight
    void addEdge(int from, int to, double weight);
    
    // получить вес ребра между вершинами from и to
    double getEdgeWeight(int from, int to) const;
    
    // проверить, существует ли ребро между вершинами
    bool hasEdge(int from, int to) const;
    
    // получить количество вершин
    int getNumVertices() const;
    
    // получить матрицу смежности
    const std::vector<std::vector<double>>& getAdjMatrix() const;
    
    // вывести матрицу смежности на экран
    void printAdjMatrix() const;
    
    // вывести список рёбер
    void printEdges() const;
};

#endif // GRAPH_H
