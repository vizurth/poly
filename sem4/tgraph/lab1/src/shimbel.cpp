#include "shimbel.h"

/*
    LOOK: initializeMatrix()
    Инициализируем единичную матрицу (k=0)
*/
template <typename T>
void ShimbelSolver<T>::initializeMatrix() {
	currentMatrix.assign(n, vector<T>(n, T{}));
	for (int i = 0; i < n; i++) {
		currentMatrix[i][i] = static_cast<T>(1);
	}
}

/*
    LOOK: multiplyMatrix(bool findMin)
    Умножаем currentMatrix на матрицу смежности графа
    findMin = true  → min (кратчайшие пути)
    findMin = false → max (длиннейшие пути)
*/
template <typename T>
void ShimbelSolver<T>::multiplyMatrix(bool findMin) {
	const T INF = getINF();
	const T empty = findMin ? INF : -INF;

	auto adj = graph.getAdjMatrix();
	vector<vector<T>> next(n, vector<T>(n, empty));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int m = 0; m < n; m++) {
				if (currentMatrix[i][m] == empty || adj[m][j] == T{})
					continue;

				T candidate = currentMatrix[i][m] + adj[m][j];

				if (findMin) {
					next[i][j] = std::min(next[i][j], candidate);
				} else {
					next[i][j] = std::max(next[i][j], candidate);
				}
			}
		}
	}

	currentMatrix = next;
}

/*
    LOOK: ShimbelSolver(const Graph<T>& g)
    Конструктор — инициализируем единичную матрицу
*/
template <typename T>
ShimbelSolver<T>::ShimbelSolver(const Graph<T> &g)
    : graph(g), n(g.getNumVertices()) {
	initializeMatrix();
}

/*
    LOOK: compute(int k, bool findMin)
    Основная логика — вычисляем матрицу путей длиной ровно k рёбер
*/
template <typename T>
vector<vector<T>> ShimbelSolver<T>::compute(int k, bool findMin) {
	if (k == 0) {
		initializeMatrix();
		return currentMatrix;
	}

	const T INF = getINF();
	const T empty = findMin ? INF : -INF;
	auto adj = graph.getAdjMatrix();

	// Начальная матрица — матрица смежности
	currentMatrix.assign(n, vector<T>(n, empty));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (adj[i][j] != T{}) {
				currentMatrix[i][j] = adj[i][j];
			}
		}
	}

	// k-1 раз умножаем на матрицу смежности
	for (int step = 1; step < k; step++) {
		multiplyMatrix(findMin);
	}

	return currentMatrix;
}

/*
    LOOK: computeShortestPaths(int k)
    Обёртка для поиска кратчайших путей
*/
template <typename T>
vector<vector<T>> ShimbelSolver<T>::computeShortestPaths(int k) {
	return compute(k, true);
}

/*
    LOOK: computeLongestPaths(int k)
    Обёртка для поиска длиннейших путей
*/
template <typename T>
vector<vector<T>> ShimbelSolver<T>::computeLongestPaths(int k) {
	return compute(k, false);
}

/*
    LOOK: printShimbelMatrix()
    Статический метод — выводим матрицу
*/
template <typename T>
void ShimbelSolver<T>::printShimbelMatrix(const vector<vector<T>> &matrix,
                                          const string &title) {
	const T INF = numeric_limits<T>::max() / 2;
	int n = matrix.size();

	cout << "\n── " << title << " ─────────────────────\n";
	cout << setw(8) << " ";
	for (int j = 0; j < n; j++) {
		cout << setw(8) << j;
	}
	cout << "\n";

	for (int i = 0; i < n; i++) {
		cout << setw(8) << i;
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] >= INF || matrix[i][j] <= -INF) {
				cout << setw(8) << "-";
			} else {
				cout << setw(8) << fixed << setprecision(2) << matrix[i][j];
			}
		}
		cout << "\n";
	}
}