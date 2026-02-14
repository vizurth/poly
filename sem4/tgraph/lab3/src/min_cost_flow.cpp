#include "../include/min_cost_flow.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>

// конструктор
MinCostFlow::MinCostFlow(int vertices) : numVertices(vertices) {
    capacity.resize(vertices, std::vector<int>(vertices, 0));
    cost.resize(vertices, std::vector<double>(vertices, 0.0));
    flow.resize(vertices, std::vector<int>(vertices, 0));
}

// поиск кратчайшего пути с минимальной стоимостью (модифицированный Беллман-Форд)
bool MinCostFlow::findMinCostPath(int source, int sink, std::vector<int>& parent, std::vector<double>& minCost) {
    const double INF = std::numeric_limits<double>::max();
    minCost.assign(numVertices, INF);
    parent.assign(numVertices, -1);
    std::vector<bool> inQueue(numVertices, false);
    
    minCost[source] = 0.0;
    
    // используем очередь для SPFA (Shortest Path Faster Algorithm)
    std::vector<int> queue;
    queue.push_back(source);
    inQueue[source] = true;
    
    // обрабатываем вершины в очереди
    while (!queue.empty()) {
        int u = queue.front();
        queue.erase(queue.begin());
        inQueue[u] = false;
        
        // проверяем все соседние вершины
        for (int v = 0; v < numVertices; v++) {
            // если есть остаточная пропускная способность
            int residualCapacity = capacity[u][v] - flow[u][v];
            
            if (residualCapacity > 0) {
                double newCost = minCost[u] + cost[u][v];
                
                // если нашли путь с меньшей стоимостью
                if (newCost < minCost[v]) {
                    minCost[v] = newCost;
                    parent[v] = u;
                    
                    // добавляем в очередь, если её там ещё нет
                    if (!inQueue[v]) {
                        queue.push_back(v);
                        inQueue[v] = true;
                    }
                }
            }
        }
    }
    
    // проверяем, достижим ли сток
    return minCost[sink] != INF;
}

// найти поток минимальной стоимости заданной величины
double MinCostFlow::minCostMaxFlow(const Graph<int>& capacityGraph, 
                                   const Graph<double>& costGraph,
                                   int source, 
                                   int sink, 
                                   int flowValue) {
    // инициализируем матрицы пропускных способностей и стоимостей
    const auto& capMatrix = capacityGraph.getAdjMatrix();
    const auto& costMatrix = costGraph.getAdjMatrix();
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            capacity[i][j] = capMatrix[i][j];
            cost[i][j] = costMatrix[i][j];
        }
    }
    
    // обнуляем матрицу потоков
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            flow[i][j] = 0;
        }
    }
    
    double totalCost = 0.0;
    int currentFlow = 0;
    std::vector<int> parent(numVertices);
    std::vector<double> minCost(numVertices);
    
    // пока не достигнем нужной величины потока и есть пути
    while (currentFlow < flowValue && findMinCostPath(source, sink, parent, minCost)) {
        // найти минимальную остаточную пропускную способность на пути
        int pathFlow = flowValue - currentFlow; // не больше оставшегося нужного потока
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            int residualCapacity = capacity[u][v] - flow[u][v];
            pathFlow = std::min(pathFlow, residualCapacity);
        }
        
        // обновить потоки и вычислить стоимость
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
            totalCost += pathFlow * cost[u][v];
        }
        
        currentFlow += pathFlow;
    }
    
    // если не удалось достичь требуемой величины потока
    if (currentFlow < flowValue) {
        std::cout << "\nВнимание: невозможно пропустить поток величиной " << flowValue << "\n";
        std::cout << "Максимально возможный поток: " << currentFlow << "\n";
    }
    
    return totalCost;
}

// получить матрицу потоков
const std::vector<std::vector<int>>& MinCostFlow::getFlow() const {
    return flow;
}

// вывести матрицу потоков и общую стоимость
void MinCostFlow::printFlow(double totalCost) const {
    std::cout << "\nматрица потоков минимальной стоимости:\n";
    
    // вывести номера столбцов
    std::cout << "    ";
    for (int i = 0; i < numVertices; i++) {
        std::cout << std::setw(6) << i;
    }
    std::cout << "\n";
    
    // вывести строки матрицы
    for (int i = 0; i < numVertices; i++) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < numVertices; j++) {
            if (flow[i][j] > 0) {
                std::cout << std::setw(6) << flow[i][j];
            } else {
                std::cout << std::setw(6) << "-";
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\nобщая стоимость потока: " << std::fixed << std::setprecision(2) << totalCost << "\n";
}
