#include "generator.h"

#include <algorithm>
#include <cmath>
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
    LOOK: void weibullShuffle(vector<int>& vertices)
    Перемешиваем вектор вершин с помощью распределения Вейбулла для генерации
   случайных индексов
*/
template <typename T>
void Generator<T>::weibullShuffle(vector<int> &vertices) {
	int n = vertices.size();
	for (int i = n - 1; i > 0; i--) {
		int j = randomIndex(i + 1);
		swap(vertices[i], vertices[j]);
	}
}

/*
    LOOK: Graph<T> generateGraph()
    Генерируем граф на основе переданных параметров в config
*/
template <typename T>
Graph<T> Generator<T>::generateGraph() {
	Graph<T> graph(numVertices);

	int maxDeg = max(1, static_cast<int>(log(numVertices)));
	vector<int> degrees(numVertices);
	for (int i = 0; i < numVertices; i++) {
		int raw = static_cast<int>(distributionForStructure.generate());
		degrees[i] = max(1, min(raw, maxDeg));
	}

	vector<int> perm(numVertices);
	iota(perm.begin(), perm.end(), 0);
	weibullShuffle(perm);

	for (int i = 1; i < numVertices; i++) {
		int j = randomIndex(i);
		T weight = generateWeight(weightMode);
		graph.addEdge(perm[j], perm[i], weight);
		if (!directed) {
			graph.addEdge(perm[i], perm[j], weight);
		}
	}

	for (int i = 0; i < numVertices; i++) {
		int edgesToAdd = degrees[i];
		vector<int> possibleTargets;

		for (int j = i + 1; j < numVertices; j++) {
			if (!graph.hasEdge(perm[i], perm[j])) {
				possibleTargets.push_back(j);
			}
		}

		if (!possibleTargets.empty()) {
			weibullShuffle(possibleTargets);

			for (int k = 0; k < edgesToAdd && k < (int)possibleTargets.size(); k++) {
				int target = possibleTargets[k];
				T weight = generateWeight(weightMode);
				graph.addEdge(perm[i], perm[target], weight);
				if (!directed) {
					graph.addEdge(perm[target], perm[i], weight);
				}
			}
		}
	}

	return graph;
}