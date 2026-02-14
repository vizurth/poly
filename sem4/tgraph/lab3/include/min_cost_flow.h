#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H

#include "../../common/graph/graph.h"
#include <vector>

// алгоритм нахождения потока минимальной стоимости заданной величины
// использует метод последовательных кратчайших путей
class MinCostFlow {
private:
    int numVertices;                                  // количество вершин в графе
    std::vector<std::vector<int>> capacity;          // матрица пропускных способностей
    std::vector<std::vector<double>> cost;           // матрица стоимостей
    std::vector<std::vector<int>> flow;              // матрица потоков
    
    // найти кратчайший путь с минимальной стоимостью с учётом текущих потоков
    // использует модифицированный алгоритм Беллмана-Форда
    bool findMinCostPath(int source, int sink, std::vector<int>& parent, std::vector<double>& minCost);

public:
    explicit MinCostFlow(int vertices);
    
    // найти поток минимальной стоимости заданной величины flowValue
    double minCostMaxFlow(const Graph<int>& capacityGraph, 
                         const Graph<double>& costGraph,
                         int source, 
                         int sink, 
                         int flowValue);
    
    // получить матрицу потоков
    const std::vector<std::vector<int>>& getFlow() const;
    
    // вывести матрицу потоков и общую стоимость
    void printFlow(double totalCost) const;
};

#endif // MIN_COST_FLOW_H
