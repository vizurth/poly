#ifndef TSP_H
#define TSP_H

#include "../../common/graph/graph.h"
#include <vector>
#include <string>
#include <limits>

// структура для хранения результата TSP
template <typename T>
struct TSPResult {
    std::vector<int> path;     // путь (последовательность вершин)
    T cost;                    // суммарная стоимость пути
    
    TSPResult() : cost(std::numeric_limits<T>::max()) {}
    TSPResult(const std::vector<int>& p, T c) : path(p), cost(c) {}
};

// класс для решения задачи коммивояжера (TSP)
template <typename T>
class TSP {
private:
    Graph<T>* graph;
    int numVertices;
    std::vector<TSPResult<T>> allCycles;  // все найденные гамильтоновы циклы
    
    // вспомогательная функция для полного перебора (backtracking)
    void findAllHamiltonianCycles(std::vector<int>& path, std::vector<bool>& visited, int pos);
    
    // вычислить стоимость цикла
    T calculateCycleCost(const std::vector<int>& cycle) const;
    
    // применить 2-opt улучшение к пути
    void twoOptImprove(std::vector<int>& path, T& cost);

public:
    // конструктор
    explicit TSP(Graph<T>* g);
    
    // решить TSP полным перебором (найти все гамильтоновы циклы)
    TSPResult<T> solveBruteForce();
    
    // решить TSP жадным алгоритмом (ближайший сосед)
    TSPResult<T> solveGreedy(int startVertex = 0);
    
    // решить TSP с 2-opt улучшением (начиная с жадного решения)
    TSPResult<T> solve2Opt(int startVertex = 0);
    
    // получить все найденные циклы
    const std::vector<TSPResult<T>>& getAllCycles() const;
    
    // записать все циклы в файл
    void writeCyclesToFile(const std::string& filename) const;
    
    // вывести топ N лучших циклов
    void printTopCycles(int topN) const;
};

#endif // TSP_H
