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

public:

	DAGGenerator(NormalDistribution& dist); // конструктор: принимает распределение для генерации весов (seed генерируется автоматически)
	DAGGenerator(NormalDistribution& dist, unsigned int seed); // конструктор: принимает распределение для генерации весов и зерно для генератора случайных чисел
	
	Graph<T> generateDAG(int numVertices, bool isDirected = true); // сгенерировать связный граф с заданным количеством вершин (по умолчанию ориентированный)
	bool isConnected(const Graph<T>& graph); // проверить, что граф связный
};

#include "dag_generator.cpp"