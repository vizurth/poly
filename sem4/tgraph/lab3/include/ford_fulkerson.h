#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include "../../common/graph/graph.h"
#include <vector>

// алгоритм Форда-Фалкерсона (реализация Эдмондса-Карпа с BFS)
// для нахождения максимального потока в сети
class FordFulkerson {
private:
    int numVertices;
    std::vector<std::vector<int>> residualGraph;  // остаточная сеть
    std::vector<std::vector<int>> flow;  // матрица потоков
    
    // поиск увеличивающего пути с помощью BFS
    // возвращает true если путь найден, и заполняет parent
    bool bfs(int source, int sink, std::vector<int>& parent);
    
public:
    // конструктор
    FordFulkerson(int vertices);
    
    // найти максимальный поток от source к sink
    // capacity - матрица пропускных способностей
    // возвращает величину максимального потока
    int maxFlow(const Graph<int>& capacity, int source, int sink);
    
    // получить матрицу потоков после выполнения maxFlow
    const std::vector<std::vector<int>>& getFlow() const;
    
    // вывести матрицу потоков
    void printFlow() const;
};

#endif // FORD_FULKERSON_H
