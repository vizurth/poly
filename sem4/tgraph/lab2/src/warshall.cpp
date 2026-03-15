
#include "warshall.h"

template <typename T>
FloydWarshall<T>::FloydWarshall(const Graph<T> &g)
    : graph(g), n(g.getNumVertices()) {
	initMatrices();
}

/*
    LOOK: initMatrices()
    Инициализируем матрицы расстояний и следующих вершин

    distMatrix[i][j]:
        0       если i == j
        w(i,j)  если есть ребро i → j
        +INF    если ребра нет

    nextMatrix[i][j]:
        j    если есть ребро i → j
        -1   если пути нет
*/
template <typename T>
void FloydWarshall<T>::initMatrices() {
	const T INF = getPosINF();
	auto adj = graph.getAdjMatrix();

	distMatrix.assign(n, vector<T>(n, INF));
	nextMatrix.assign(n, vector<int>(n, -1));

	for (int i = 0; i < n; i++) {
		distMatrix[i][i] = static_cast<T>(0);

		for (int j = 0; j < n; j++) {
			if (i != j && adj[i][j] != T{}) {
				distMatrix[i][j] = adj[i][j];
				nextMatrix[i][j] = j;
			}
		}
	}
}

/*
    LOOK: compute()
    Основной алгоритм Флойда-Уоршалла

    Для каждой промежуточной вершины k проверяем:
    если dist[i][k] + dist[k][j] < dist[i][j]
    то обновляем расстояние и запоминаем путь через k
*/
template <typename T>
void FloydWarshall<T>::compute() {
	const T INF = getPosINF();

	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				// пропускаем если нет пути через k
				if (distMatrix[i][k] == INF || distMatrix[k][j] == INF)
					continue;

				T newDist = distMatrix[i][k] + distMatrix[k][j];
				if (newDist < distMatrix[i][j]) {
					distMatrix[i][j] = newDist;
					nextMatrix[i][j] = nextMatrix[i][k]; // путь идёт через k
				}
			}
		}
	}
}

/*
    LOOK: getPath(int from, int to)
    Восстанавливаем путь из from в to
    используя матрицу nextMatrix
*/
template <typename T>
vector<int> FloydWarshall<T>::getPath(int from, int to) const {
	if (nextMatrix[from][to] == -1)
		return {}; // пути нет

	vector<int> path = {from};
	int current = from;

	while (current != to) {
		current = nextMatrix[current][to];
		path.push_back(current);

		// защита от зацикливания
		if (path.size() > n)
			return {};
	}

	return path;
}

/*
    LOOK: printDistMatrix()
    Выводим матрицу расстояний
*/
template <typename T>
void FloydWarshall<T>::printDistMatrix() const {
	printMatrix(distMatrix, "Матрица расстояний (Флойд-Уоршалл):");
}

/*
    LOOK: printPath(int from, int to)
    Выводим кратчайший путь и его длину
*/
template <typename T>
void FloydWarshall<T>::printPath(int from, int to) const {
	const T INF = getPosINF();

	cout << "\n── Кратчайший путь: " << from << " → " << to
	     << " ──────────────\n";

	if (distMatrix[from][to] == INF) {
		cout << "  Путь недостижим\n";
		return;
	}

	vector<int> path = getPath(from, to);
	if (path.empty()) {
		cout << "  Путь не найден\n";
		return;
	}

	// выводим путь
	cout << "  Путь: ";
	for (int i = 0; i < path.size(); i++) {
		cout << path[i];
		if (i + 1 < path.size())
			cout << " → ";
	}

	// выводим длину пути
	cout << "\n  Длина: " << fixed << setprecision(2) << distMatrix[from][to]
	     << "\n";
}