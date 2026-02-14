#include "../include/tsp.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

// конструктор
template <typename T>
TSP<T>::TSP(Graph<T>* g) : graph(g), numVertices(g->getNumVertices()) {}

// вычислить стоимость цикла
template <typename T>
T TSP<T>::calculateCycleCost(const std::vector<int>& cycle) const {
    const auto& adjMatrix = graph->getAdjMatrix();
    T cost = 0;
    
    for (size_t i = 0; i < cycle.size(); i++) {
        int from = cycle[i];
        int to = cycle[(i + 1) % cycle.size()];
        
        if (adjMatrix[from][to] == 0) {
            return std::numeric_limits<T>::max(); // нет ребра
        }
        
        cost += adjMatrix[from][to];
    }
    
    return cost;
}

// вспомогательная функция для полного перебора
template <typename T>
void TSP<T>::findAllHamiltonianCycles(std::vector<int>& path, std::vector<bool>& visited, int pos) {
    const auto& adjMatrix = graph->getAdjMatrix();
    
    // если все вершины добавлены в путь
    if (pos == numVertices) {
        // проверить, есть ли ребро от последней вершины к первой
        if (adjMatrix[path[pos - 1]][path[0]] != 0) {
            // вычислить стоимость цикла
            T cost = calculateCycleCost(path);
            allCycles.push_back(TSPResult<T>(path, cost));
        }
        return;
    }
    
    // пробуем добавить разные вершины
    for (int v = 1; v < numVertices; v++) {
        if (!visited[v] && adjMatrix[path[pos - 1]][v] != 0) {
            path[pos] = v;
            visited[v] = true;
            
            findAllHamiltonianCycles(path, visited, pos + 1);
            
            // backtrack
            visited[v] = false;
            path[pos] = -1;
        }
    }
}

// решить TSP полным перебором
template <typename T>
TSPResult<T> TSP<T>::solveBruteForce() {
    allCycles.clear();
    
    std::vector<int> path(numVertices, -1);
    std::vector<bool> visited(numVertices, false);
    
    // начинаем с вершины 0
    path[0] = 0;
    visited[0] = true;
    
    findAllHamiltonianCycles(path, visited, 1);
    
    // найти минимальный цикл
    TSPResult<T> bestResult;
    for (const auto& cycle : allCycles) {
        if (cycle.cost < bestResult.cost) {
            bestResult = cycle;
        }
    }
    
    return bestResult;
}

// решить TSP жадным алгоритмом (ближайший сосед)
template <typename T>
TSPResult<T> TSP<T>::solveGreedy(int startVertex) {
    const auto& adjMatrix = graph->getAdjMatrix();
    std::vector<bool> visited(numVertices, false);
    std::vector<int> path;
    
    int current = startVertex;
    path.push_back(current);
    visited[current] = true;
    
    // жадно выбираем ближайшую непосещённую вершину
    for (int i = 1; i < numVertices; i++) {
        int nearest = -1;
        T minDist = std::numeric_limits<T>::max();
        
        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && adjMatrix[current][v] != 0) {
                if (adjMatrix[current][v] < minDist) {
                    minDist = adjMatrix[current][v];
                    nearest = v;
                }
            }
        }
        
        if (nearest == -1) {
            // не можем продолжить, граф не полный
            return TSPResult<T>();
        }
        
        path.push_back(nearest);
        visited[nearest] = true;
        current = nearest;
    }
    
    // проверить, можно ли вернуться в начало
    if (adjMatrix[current][startVertex] == 0) {
        return TSPResult<T>();
    }
    
    T cost = calculateCycleCost(path);
    return TSPResult<T>(path, cost);
}

// применить 2-opt улучшение
template <typename T>
void TSP<T>::twoOptImprove(std::vector<int>& path, T& cost) {
    bool improved = true;
    const auto& adjMatrix = graph->getAdjMatrix();
    
    while (improved) {
        improved = false;
        
        for (size_t i = 0; i < path.size() - 1; i++) {
            for (size_t j = i + 2; j < path.size(); j++) {
                // попробовать обратить сегмент [i+1, j]
                
                // текущие рёбра: (path[i], path[i+1]) и (path[j], path[j+1])
                int a = path[i];
                int b = path[i + 1];
                int c = path[j];
                int d = path[(j + 1) % path.size()];
                
                if (adjMatrix[a][b] == 0 || adjMatrix[c][d] == 0 ||
                    adjMatrix[a][c] == 0 || adjMatrix[b][d] == 0) {
                    continue;
                }
                
                // старая стоимость этих рёбер
                T oldCost = adjMatrix[a][b] + adjMatrix[c][d];
                // новая стоимость после реверса
                T newCost = adjMatrix[a][c] + adjMatrix[b][d];
                
                if (newCost < oldCost) {
                    // реверс улучшает решение
                    std::reverse(path.begin() + i + 1, path.begin() + j + 1);
                    cost = cost - oldCost + newCost;
                    improved = true;
                }
            }
        }
    }
}

// решить TSP с 2-opt улучшением
template <typename T>
TSPResult<T> TSP<T>::solve2Opt(int startVertex) {
    // начать с жадного решения
    auto result = solveGreedy(startVertex);
    
    if (result.path.empty()) {
        return result;
    }
    
    // применить 2-opt улучшение
    twoOptImprove(result.path, result.cost);
    
    return result;
}

// получить все найденные циклы
template <typename T>
const std::vector<TSPResult<T>>& TSP<T>::getAllCycles() const {
    return allCycles;
}

// записать все циклы в файл
template <typename T>
void TSP<T>::writeCyclesToFile(const std::string& filename) const {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "ошибка: не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    
    file << "всего найдено гамильтоновых циклов: " << allCycles.size() << "\n\n";
    
    for (size_t i = 0; i < allCycles.size(); i++) {
        file << "цикл #" << (i + 1) << ":\n";
        file << "путь: ";
        for (int v : allCycles[i].path) {
            file << v << " ";
        }
        file << "-> " << allCycles[i].path[0] << "\n";
        file << "стоимость: " << std::fixed << std::setprecision(2) << allCycles[i].cost << "\n\n";
    }
    
    file.close();
    std::cout << "результаты записаны в файл: " << filename << "\n";
}

// вывести топ N лучших циклов
template <typename T>
void TSP<T>::printTopCycles(int topN) const {
    if (allCycles.empty()) {
        std::cout << "циклы не найдены\n";
        return;
    }
    
    // создать копию и отсортировать по стоимости
    auto sortedCycles = allCycles;
    std::sort(sortedCycles.begin(), sortedCycles.end(), 
              [](const TSPResult<T>& a, const TSPResult<T>& b) {
                  return a.cost < b.cost;
              });
    
    int count = std::min(topN, static_cast<int>(sortedCycles.size()));
    
    std::cout << "\nтоп " << count << " лучших циклов:\n";
    for (int i = 0; i < count; i++) {
        std::cout << "\n" << (i + 1) << ". стоимость: " << std::fixed << std::setprecision(2) 
                  << sortedCycles[i].cost << "\n";
        std::cout << "   путь: ";
        for (int v : sortedCycles[i].path) {
            std::cout << v << " ";
        }
        std::cout << "-> " << sortedCycles[i].path[0] << "\n";
    }
}

// явная инстанциация шаблонов
template class TSP<double>;
template class TSP<int>;
