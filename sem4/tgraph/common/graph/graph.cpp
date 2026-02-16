#include "graph.h"
#include <iostream>
#include <iomanip>

using namespace std;

// получить количество вершин
template<typename T>
int Graph<T>::getNumVertices() const {
	return numVertices;
}

// добавить ориентированное ребро (u, v) с весом weight
template<typename T>
void Graph<T>::addEdge(int u, int v, T weight) {
	if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
		adjMatrix[u][v] = weight;
	}
}

// получить вес ребра (u, v), 0 если ребра нет
template<typename T>
T Graph<T>::getEdge(int u, int v) const {
	if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
		return adjMatrix[u][v];
	}
	return 0;
}

// проверить наличие ребра (u, v)
template<typename T>
bool Graph<T>::hasEdge(int u, int v) const {
	if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
		return adjMatrix[u][v] != 0;
	}
	return false;
}

// получить матрицу смежности
template<typename T>
const vector<vector<T>>& Graph<T>::getAdjMatrix() const {
	return adjMatrix;
}

// вывести матрицу смежности на экран
template<typename T>	
void Graph<T>::printAdjMatrix() const {
	cout << "\nматрица смежности:\n";
	
	// вывести заголовок (номера столбцов)
	cout << "    ";
	for (int i = 0; i < numVertices; i++) {
		cout << setw(8) << i;
	}
	cout << "\n";
	
	// вывести строки матрицы
	for (int i = 0; i < numVertices; i++) {
		cout << setw(3) << i << " ";
		for (int j = 0; j < numVertices; j++) {
			if (adjMatrix[i][j] != 0) {
				cout << setw(8) << fixed << setprecision(2) << adjMatrix[i][j];
			} else {
				cout << setw(8) << "-";
			}
		}
		cout << "\n";
	}
}

// вывести список рёбер на экран
template<typename T>
void Graph<T>::printEdges() const {
	cout << "\nсписок рёбер:\n";
	
	int edgeCount = 0;
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			if (adjMatrix[i][j] != 0) {
				cout << i << " -> " << j << " (вес: " 
				     << fixed << setprecision(2) << adjMatrix[i][j] << ")\n";
				edgeCount++;
			}
		}
	}
	
	if (edgeCount == 0) {
		cout << "граф не содержит рёбер\n";
	} else {
		cout << "всего рёбер: " << edgeCount << "\n";
	}
}

// получить список рёбер, исходящих из вершины u
template<typename T>
vector<pair<int, T>> Graph<T>::getOutgoingEdges(int u) const {
	vector<pair<int, T>> edges;
	
	if (u >= 0 && u < numVertices) {
		for (int v = 0; v < numVertices; v++) {
			if (adjMatrix[u][v] != 0) {
				edges.push_back({v, adjMatrix[u][v]});
			}
		}
	}
	
	return edges;
}

// получить список рёбер, входящих в вершину v
template<typename T>
vector<pair<int, T>> Graph<T>::getIncomingEdges(int v) const {
	vector<pair<int, T>> edges;
	
	if (v >= 0 && v < numVertices) {
		for (int u = 0; u < numVertices; u++) {
			if (adjMatrix[u][v] != 0) {
				edges.push_back({u, adjMatrix[u][v]});
			}
		}
	}
	
	return edges;
}

// получить список соседей вершины v (для неориентированного графа)
template<typename T>
vector<pair<int, T>> Graph<T>::getAdj(int v) const {
	vector<pair<int, T>> neighbors;
	
	if (v >= 0 && v < numVertices) {
		// для неориентированного графа берём и исходящие и входящие рёбра
		for (int u = 0; u < numVertices; u++) {
			// исходящие рёбра
			if (adjMatrix[v][u] != 0) {
				neighbors.push_back({u, adjMatrix[v][u]});
			}
			// входящие рёбра (если это другая вершина)
			if (u != v && adjMatrix[u][v] != 0) {
				// проверяем, не добавили ли мы уже это ребро
				bool found = false;
				for (const auto& neighbor : neighbors) {
					if (neighbor.first == u) {
						found = true;
						break;
					}
				}
				if (!found) {
					neighbors.push_back({u, adjMatrix[u][v]});
				}
			}
		}
	}
	
	return neighbors;
}

// явная инстанциация шаблонов
template class Graph<double>;
template class Graph<int>;
template class Graph<float>;