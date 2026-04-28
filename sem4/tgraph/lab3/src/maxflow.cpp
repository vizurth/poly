#include "maxflow.h"
#include <limits>
#include <queue>
using namespace std;

/*
	LOOK: MaxFlow(const Graph<T> &g)
	Конструктор инициализирует остаточный граф из весовой матрицы графа.
*/
template <typename T>
MaxFlow<T>::MaxFlow(const Graph<T> &g) : n(g.getNumVertices()) {
	cap = g.getWeightMatrix();
	// отрицательные веса не имеют смысла как пропускные способности
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (cap[i][j] < T{})
				cap[i][j] = -cap[i][j];
}
/*
    LOOK: bfs(int s, int t)
    Ищем кратчайший увеличивающий путь от s до t.
    parent[v] = предыдущая вершина на пути — восстанавливаем путь из t обратно к s.
*/
template <typename T>
vector<int> MaxFlow<T>::bfs(int s, int t) const {
	vector<int> parent(n, -1);
	parent[s] = s;
	queue<int> q;
	q.push(s);

	while (!q.empty()) {
		int v = q.front();
		q.pop();
		if (v == t)
			break;
		for (int u = 0; u < n; u++) {
			if (parent[u] == -1 && cap[v][u] > T{}) {
				parent[u] = v;
				q.push(u);
			}
		}
	}

	if (parent[t] == -1)
		return {}; // пути нет

	// восстанавливаем путь от t к s и разворачиваем
	vector<int> path;
	for (int v = t; v != s; v = parent[v])
		path.push_back(v);
	path.push_back(s);
	reverse(path.begin(), path.end());
	return path;
}

/*
    LOOK: compute(int s, int t)
    Основной цикл алгоритма Эдмондса Карпа:
    1. Найти увеличивающий путь через BFS
    2. Найти минимальную пропускную способность на пути (узкое место)
    3. Обновить остаточный граф: уменьшить прямые рёбра, увеличить обратные
    4. Добавить к потоку
    5. Повторять до тех пор пока пути есть
*/
template <typename T>
T MaxFlow<T>::compute(int s, int t) {
	printMatrix(cap, "Начальный остаточный граф (пропускные способности):");
	cout << "\n";
	T maxFlow = T{};
	int iter = 0;
	while (true) {
		vector<int> path = bfs(s, t);
		if (path.empty())
			break;
		cout << iter + 1 << ".  Найден увеличивающий путь: ";
		for (int i = 0; i < (int)path.size(); i++) {
			if (i > 0)
				cout << " => ";
			cout << path[i];
		}
		cout << "\n";

		// push = узкое место на пути (минимальная пропускная способность)
		T push = numeric_limits<T>::max();
		for (int i = 0; i + 1 < (int)path.size(); i++)
			push = min(push, cap[path[i]][path[i + 1]]);
		cout << "    Узкое место (минимальная пропускная способность на пути): "
		     << push << "\n";

		// обновляем остаточный граф
		for (int i = 0; i + 1 < (int)path.size(); i++) {
			cap[path[i]][path[i + 1]] -= push; // уменьшаем прямое ребро
			cap[path[i + 1]][path[i]] += push; // увеличиваем обратное
		}
		// printMatrix(cap, "Остаточный граф после обновления пропускных способностей:");

		maxFlow += push;
		iter++;
	}

	return maxFlow;
}

/*
	LOOK: printResult(T flow, int s, int t)
	Выводим результат: максимальный поток и соответствующие вершины.
*/
template <typename T>
void MaxFlow<T>::printResult(T flow, int s, int t) const {
	cout << "  Источник: " << s << ",  Сток: " << t << "\n";
	cout << "  Максимальный поток: " << flow << "\n";
}
