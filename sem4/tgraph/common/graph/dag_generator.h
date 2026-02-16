#pragma once 

#include "graph.h"
#include "../distributions/normal.h"
#include <vector>
#include <random>

template<typename T>
class DAGGenerator {
private:
	NormalDistribution& distribution; // распределение для генерации весов рёбер
	std::mt19937 generator; // генератор случайных чисел

	bool isConnected(const Graph<T>& graph); // проверить, что граф связный

public:
	DAGGenerator(NormalDistribution& dist, unsigned int seed); // конструктор: принимает распределение для генерации весов и зерно для генератора случайных чисел
	
	Graph<T> generateDAG(int numVertices); // сгенерировать связный ациклический граф с заданным количеством вершин
};