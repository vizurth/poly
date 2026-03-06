#pragma once

#include "../distribution/weibull.h"
#include "graph.h"
#include <vector>

enum class WeightType { POSITIVE, NEGATIVE, MIXED };

struct GeneratorConfig {
	int numVertices;
	bool isDirected;
	WeightType weightType;
};

template <typename T>
class Generator {
  private:
	WeibullDistribution distributionForWeights;
	WeibullDistribution distributionForStructure;

	// Конфиг графа
	int numVertices;
	bool directed;
	WeightType weightMode;

	int randomIndex(int n);
	bool isNegative();

	T generateWeight(WeightType weightType);
	void weibullShuffle(std::vector<int> &vertices);

  public:
	Generator(int numVertices, bool directed, WeightType weightMode,
	          double structScale, double structShape, double weightScale,
	          double weightShape)
	    : distributionForWeights(weightScale, weightShape),
	      distributionForStructure(structScale, structShape),
	      numVertices(numVertices), directed(directed), weightMode(weightMode) {
		if (numVertices < 2) {
			throw std::invalid_argument("Граф должен иметь хотя бы 2 вершины");
		}
	}

	Graph<T> generateGraph();
};

#include "generator.cpp"
