#include "mincost.h"
#include "common/utils/utils.h"
#include "lab2/src/warshall.h"
#include <iomanip>
#include <limits>
using namespace std;

/*
	LOOK: MinCostFlow(const Graph<T> &g, const vector<vector<T>> &costMatrix)
	Конструктор инициализирует остаточный граф из весовой матрицы графа и
	заданной матрицы стоимостей.
*/
template <typename T>
MinCostFlow<T>::MinCostFlow(const Graph<T> &g,
                            const vector<vector<T>> &costMatrix)
    : n(g.getNumVertices()) {
	auto adj = g.getAdjMatrix();
	auto weights = g.getWeightMatrix();

	cap.assign(n, vector<T>(n, T{}));
	cost.assign(n, vector<T>(n, T{}));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (adj[i][j] != 0) {
				cap[i][j] =
				    (weights[i][j] >= T{}) ? weights[i][j] : -weights[i][j];
				cost[i][j] = costMatrix[i][j];
				cost[j][i] = -costMatrix[i][j];
			}
		}
	}
}

/*
	LOOK: findMinCostPath(int s, int t)
	Строим остаточный граф и находим путь s->t минимальной стоимости c помощью матрицы cost и алгоритма Флойда-Уоршалла. Возвращаем {путь, стоимость}.
*/
template <typename T>
pair<vector<int>, T> MinCostFlow<T>::findMinCostPath(int s, int t) const {
	const T INF_VAL = numeric_limits<T>::max() / 2;

	Graph<T> residual(n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (cap[i][j] > T{})
				residual.addEdge(i, j, cost[i][j]);

	FloydWarshall<T> fw(residual);
	fw.compute();

	const auto &dist = fw.getDistMatrix();
	if (dist[s][t] >= INF_VAL)
		return {{}, T{}};

	return {fw.getPath(s, t), dist[s][t]};
}

/*
    LOOK: printIterationHeader(int iter, T flow, T targetFlow)
    Выводим заголовок для каждой итерации алгоритма.
*/
template <typename T>
static void printIterationHeader(int iter, T flow, T targetFlow) {
	cout << "==  Итерация " << iter << "   (пущено: " << flow << " / "
	     << targetFlow << ") ==\n";
}

/*
	LOOK: compute(int s, int t, T targetFlow)
	Основной цикл алгоритма минимального стоимостного потока:
	1. Найти путь минимальной стоимости через findMinCostPath
	2. Найти узкое место на пути (минимальная пропускная способность)
	3. Обновить остаточный граф
	4. Повторять до достижения целевого потока или невозможности продолжать
*/
template <typename T>
pair<T, T> MinCostFlow<T>::compute(int s, int t, T targetFlow) {
	T flow = T{};
	T totalCost = T{};
	int iter = 1;

	while (flow < targetFlow) {
		printIterationHeader(iter++, flow, targetFlow);
		printMatrix(cap, "Остаточные пропускные способности:");

		// стоимости только там где есть пропускная способность
		const T INF_VAL = numeric_limits<T>::max() / 2;
		vector<vector<T>> resCost(n, vector<T>(n, INF_VAL));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (cap[i][j] > T{})
					resCost[i][j] = cost[i][j];
		printMatrix(resCost, "Стоимости рёбер остаточной сети:");

		auto [path, pathCost] = findMinCostPath(s, t);

		if (path.empty()) {
			cout << "\n  Путь из " << s << " в " << t
			     << " не найден — алгоритм завершён.\n";
			break;
		}

		cout << "\n  Кратчайший путь:  ";
		for (int i = 0; i < (int)path.size(); i++) {
			if (i > 0)
				cout << " => ";
			cout << path[i];
		}
		cout << "   (стоимость единицы потока: " << pathCost << ")\n";

		T push = targetFlow - flow;
		int bottleneckU = path[0], bottleneckV = path[1];
		for (int i = 0; i + 1 < (int)path.size(); i++) {
			if (cap[path[i]][path[i + 1]] < push) { // ищем узкое место минимальную пропуск способностьь
				push = cap[path[i]][path[i + 1]];
				bottleneckU = path[i];
				bottleneckV = path[i + 1];
			}
		}
		cout << "  Узкое место: ребро " << bottleneckU << " => " << bottleneckV
		     << "   (пропускная способность: " << push << ")\n";

		// обновление остаточного графа
		for (int i = 0; i + 1 < (int)path.size(); i++) {
			int u = path[i], v = path[i + 1];
			cap[u][v] -= push;
			cap[v][u] += push;
		}

		flow += push;
		totalCost += push * pathCost;

		cout << "  Пущено: " << push << "      Итого поток: " << flow << "      Итого стоимость: " << totalCost << "\n";
	}

	return {flow, totalCost};
}


/*
	LOOK: printResult(T flow, T totalCost, int s, int t)
	Выводим итоговый достигнутый поток и его стоимость.
*/
template <typename T>
void MinCostFlow<T>::printResult(T flow, T totalCost, T target, int s,
                                 int t) const {
	cout << "\n== Итог ==\n";
	cout << "  Источник: " << s << "   Сток: " << t << "\n";
	cout << "  Целевой поток [2/3 * max]: " << target << "\n";
	cout << "  Достигнутый поток:         " << flow << "\n";
	cout << "  Минимальная стоимость:     " << totalCost << "\n";
}