#include "../include/ford_fulkerson.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <limits>

// конструктор
FordFulkerson::FordFulkerson(int vertices) : numVertices(vertices) {
    residualGraph.resize(vertices, std::vector<int>(vertices, 0));
    flow.resize(vertices, std::vector<int>(vertices, 0));
}

// поиск увеличивающего пути с помощью BFS (алгоритм Эдмондса-Карпа)
bool FordFulkerson::bfs(int source, int sink, std::vector<int>& parent) {
    std::vector<bool> visited(numVertices, false);
    std::queue<int> q;
    
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        // проверяем все соседние вершины
        for (int v = 0; v < numVertices; v++) {
            // если вершина не посещена и есть остаточная пропускная способность
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                
                // если достигли стока, путь найден
                if (v == sink) {
                    return true;
                }
            }
        }
    }
    
    // путь не найден
    return false;
}

// найти максимальный поток от source к sink
int FordFulkerson::maxFlow(const Graph<int>& capacity, int source, int sink) {
    // инициализируем остаточную сеть с пропускными способностями
    const auto& capMatrix = capacity.getAdjMatrix();
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            residualGraph[i][j] = capMatrix[i][j];
        }
    }
    
    // обнуляем матрицу потоков
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            flow[i][j] = 0;
        }
    }
    
    std::vector<int> parent(numVertices);
    int maxFlowValue = 0;
    
    // пока существует увеличивающий путь от source к sink
    while (bfs(source, sink, parent)) {
        // найти минимальную остаточную пропускную способность на пути
        int pathFlow = std::numeric_limits<int>::max();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }
        
        // обновить остаточную сеть и матрицу потоков
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }
        
        maxFlowValue += pathFlow;
    }
    
    return maxFlowValue;
}

// получить матрицу потоков
const std::vector<std::vector<int>>& FordFulkerson::getFlow() const {
    return flow;
}

// вывести матрицу потоков
void FordFulkerson::printFlow() const {
    std::cout << "\nматрица потоков:\n";
    
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
}
