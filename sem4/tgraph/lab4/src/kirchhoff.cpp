#include "kirchhoff.h"
#include <iomanip>
#include <iostream>
using namespace std;

/*
    Матрица Лапласа (для неориентированного графа):
      L[i][i] = степень вершины i
      L[i][j] = -1 если ребро (i,j) есть, иначе 0
*/
long long Kirchhoff::count(const Graph<int> &g) {
	int n = g.getNumVertices();
	if (n <= 1) return 1;

	auto adj = g.getAdjMatrix();

	// строим Лапласиан
	vector<vector<long long>> L(n, vector<long long>(n, 0));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j && (adj[i][j] != 0 || adj[j][i] != 0))
				L[i][j] = -1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (L[i][j] == -1)
				L[i][i]++;

	// кофактор: удаляем строку и столбец n-1
	int m = n - 1;
	vector<vector<long long>> mat(m, vector<long long>(m));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			mat[i][j] = L[i][j];

	// алгоритм Барейсса — точный целочисленный det
	for (int k = 0; k < m; k++) {
		int pivot = -1;
		for (int i = k; i < m; i++)
			if (mat[i][k] != 0) { pivot = i; break; }
		if (pivot == -1) return 0;
		if (pivot != k) swap(mat[k], mat[pivot]);

		for (int i = k + 1; i < m; i++) {
			for (int j = k + 1; j < m; j++) {
				mat[i][j] = mat[k][k] * mat[i][j] - mat[i][k] * mat[k][j];
				if (k > 0) mat[i][j] /= mat[k - 1][k - 1];
			}
			mat[i][k] = 0;
		}
	}

	return abs(mat[m - 1][m - 1]);
}

void Kirchhoff::printLaplacian(const vector<vector<int>> &L, int n) {
	const int W = 4;
	cout << "\n  Матрица Лапласа:\n";
	cout << "     ";
	for (int j = 0; j < n; j++) cout << setw(W) << j;
	cout << "\n";
	for (int i = 0; i < n; i++) {
		cout << "  " << setw(2) << i << " [";
		for (int j = 0; j < n; j++) cout << setw(W) << L[i][j];
		cout << " ]\n";
	}
}

void Kirchhoff::printResult(long long cnt) {
	cout << "\n  Число остовных деревьев: " << cnt << "\n";
}
