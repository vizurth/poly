#pragma once 

#include "graph.h"
#include "../distributions/weibull.h"
#include <vector>
#include <random>

using namespace std;

enum class WeightType {
	POSITIVE,   // только положительные веса
	NEGATIVE,   // только отрицательные веса
	MIXED       // смешанные (положительные и отрицательные)
};

template<typename T>
class DAGGenerator {
private:
	WeibullDistribution& distribution; // распределение для генерации весов рёбер
	mt19937 generator; // генератор случайных чисел

public:
	// конструкторы
	DAGGenerator(WeibullDistribution& dist);
	DAGGenerator(WeibullDistribution& dist, unsigned int seed);
	
	// функции
	Graph<T> generateDAG(int numVertices, bool isDirected = true, WeightType weightType = WeightType::POSITIVE); 
	bool isConnected(const Graph<T>& graph);
};

#include "dag_generator.cpp"