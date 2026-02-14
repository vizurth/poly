#ifndef HAMILTON_H
#define HAMILTON_H

#include "../../common/graph/graph.h"
#include <vector>
#include <utility>

// класс для работы с гамильтоновыми графами
template <typename T>
class HamiltonGraph {
private:
    Graph<T>* graph;
    int numVertices;
    
    // вспомогательная функция для поиска гамильтонова цикла (backtracking)
    bool hamiltonianCycleUtil(std::vector<int>& path, std::vector<bool>& visited, int pos);
    
    // проверить, можно ли добавить вершину v в позицию pos в пути
    bool isSafe(int v, const std::vector<int>& path, int pos) const;

public:
    // конструктор
    explicit HamiltonGraph(Graph<T>* g);
    
    // проверить, является ли граф гамильтоновым (существует цикл через все вершины)
    bool isHamiltonian();
    
    // найти гамильтонов цикл (если существует)
    std::vector<int> findHamiltonianCycle();
    
    // модифицировать граф до гамильтонова (добавить минимум рёбер)
    // возвращает список добавленных рёбер: пары (from, to)
    std::vector<std::pair<int, int>> makeHamiltonian();
};

#endif // HAMILTON_H
