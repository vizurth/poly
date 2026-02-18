#pragma once 

#include "graph.h"
#include "../distributions/normal.h"
#include <vector>
#include <random>

using namespace std;

template<typename T>
class DAGGenerator {
private:
	NormalDistribution& distribution; // распределение для генерации весов рёбер
	mt19937 generator; // генератор случайных чисел

public:
	// конструкторы
	DAGGenerator(NormalDistribution& dist);
	DAGGenerator(NormalDistribution& dist, unsigned int seed);
	
	// функции
	Graph<T> generateDAG(int numVertices, bool isDirected = true); 
	bool isConnected(const Graph<T>& graph);
};

#include "dag_generator.cpp"