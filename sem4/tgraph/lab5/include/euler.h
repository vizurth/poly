#ifndef EULER_H
#define EULER_H

#include "../../common/graph/graph.h"
#include <vector>
#include <utility>

// класс для работы с эйлеровыми графами
template <typename T>
class EulerGraph {
private:
    Graph<T>* graph;
    int numVertices;
    
    // проверить связность графа (обход в глубину)
    bool isConnected() const;
    
    // подсчитать степень вершины
    int getDegree(int vertex) const;
    
    // получить список вершин с нечётной степенью
    std::vector<int> getOddDegreeVertices() const;
    
    // построить эйлеров цикл алгоритмом хирхольцера
    void hierholzerDFS(int vertex, std::vector<std::vector<int>>& adjList, std::vector<int>& circuit);

public:
    // конструктор
    explicit EulerGraph(Graph<T>* g);
    
    // проверить, является ли граф эйлеровым (цикл)
    bool isEulerian() const;
    
    // проверить, существует ли эйлеров путь (не цикл)
    bool hasEulerianPath() const;
    
    // модифицировать граф до эйлерова (добавить минимум рёбер)
    // возвращает список добавленных рёбер: пары (from, to)
    std::vector<std::pair<int, int>> makeEulerian();
    
    // построить эйлеров цикл (алгоритм хирхольцера)
    std::vector<int> findEulerianCircuit();
    
    // вывести информацию о степенях вершин
    void printDegrees() const;
};

#endif // EULER_H
