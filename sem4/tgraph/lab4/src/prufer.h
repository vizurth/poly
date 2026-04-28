#pragma once

#include "kruskal.h"
#include <vector>
using namespace std;

struct PruferCode {
	vector<int> seq;     // последовательность Прюфера (n-1 элементов)
	vector<int> weights; // вес ребра, соответствующего каждому элементу
	int n = 0;           // число вершин дерева
};

/*
    Кодирование и декодирование кода Прюфера (вариант с n-1 элементами).

    Кодирование:
      для каждой из n-1 итераций: взять висячую вершину с min номером,
      записать её соседа в код, удалить вершину.

    Декодирование:
      для каждой позиции кода: взять min вершину, не входящую в остаток кода,
      добавить ребро к текущему элементу кода, удалить вершину.
*/
class Prufer {
  public:
	static PruferCode encode(const vector<Edge> &mst, int n);
	static vector<Edge> decode(const PruferCode &code);

	static void printCode(const PruferCode &code);
	static void printEdges(const vector<Edge> &edges, const string &title);
};
