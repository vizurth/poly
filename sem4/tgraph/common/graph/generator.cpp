#include "generator.h"

#include <algorithm>
#include <numeric>

/*
    LOOK: int randomIndex(int n)
    Генерируем случайный индекс от 0 до n-1 с помощью распределения Вейбулла
*/
template <typename T>
int Generator<T>::randomIndex(int n) {
	return static_cast<int>(distributionForStructure.generate()) % n;
}

/*
    LOOK: bool isNegative()
    Генерируем случайное булево значение для определения знака веса
*/
template <typename T>
bool Generator<T>::isNegative() {
	double a = distributionForWeights.generate();
	double b = distributionForWeights.generate();
	return a > b;
}

/*
    LOOK: Graph<T> generateGraph(WeightType weightType)
    Генерируем граф на основе переданных параметров в config
*/
template <typename T>
T Generator<T>::generateWeight(WeightType weightType) {
	double weight = distributionForWeights.generate();
	if (weightType == WeightType::NEGATIVE) {
		return static_cast<T>(-weight);
	} else if (weightType == WeightType::MIXED) {
		return isNegative() ? static_cast<T>(-weight) : static_cast<T>(weight);
	}
	return static_cast<T>(weight);
}

/*
    LOOK: Graph<T> generateGraph()
    Генерируем граф на основе переданных параметров в config
*/
template <typename T>
Graph<T> Generator<T>::generateGraph() {
	Graph<T> graph(numVertices);

	vector<int> vertices(numVertices);
	std::iota(vertices.begin(), vertices.end(), 0);

	for (int i = 1; i < numVertices; i++) {
		int j = randomIndex(i); // i > j
		int from = vertices[j];
		int to = vertices[i];
		T weight = generateWeight(weightMode);

		graph.addEdge(from, to, weight);

		if (!directed) {
			graph.addEdge(to, from, weight);
		}
	}

	if (directed) {
		int maxExtraEdges = numVertices * 2;
		int extraEdges = static_cast<int>(distributionForStructure.generate()) %
		                 (maxExtraEdges + 1);
		for (int k = 0; k < extraEdges; k++) {
			int i = 1 + static_cast<int>(distributionForStructure.generate()) %
			                (numVertices - 1);
			int j = static_cast<int>(distributionForStructure.generate()) % i; // i > j
			int from = vertices[j];
			int to = vertices[i];
			if (!graph.hasEdge(from, to)) {
				T weight = generateWeight(weightMode);
				graph.addEdge(from, to, weight);
			}
		}
	}
	return graph;
}