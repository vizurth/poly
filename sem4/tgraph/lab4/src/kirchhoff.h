#pragma once

#include "common/graph/graph.h"
#include <vector>
using namespace std;

/*
    Матричная теорема Кирхгофа (Matrix-Tree Theorem).
    Число остовных деревьев = любой кофактор матрицы Лапласа.
    Вычисляется через алгоритм Барейсса (точная целочисленная арифметика).
*/
class Kirchhoff {
  public:
	static long long count(const Graph<int> &g);
	static void printLaplacian(const vector<vector<int>> &L, int n);
	static void printResult(long long cnt);
};
