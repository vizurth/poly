#include "lab1/include/route_counter.h"

/*
	LOOK: RouteCounter(const Graph<T>& g)
	Конструктор класса
*/
template <typename T>
RouteCounter<T>::RouteCounter(const Graph<T>& g) : graph(g), n(g.getNumVertices()) {
    visited.resize(n, false);
}


/*
	LOOK: void findAllPaths(current, target)
	С помощью backtracking опходим граф и сохраняем все пусти в allRoutes
*/
template <typename T>
void RouteCounter<T>::findAllPaths(int current, int target) {
    // добавляем текущую вершину в путь
    currentPath.push_back(current);
    visited[current] = true;
    
    // если достигли целевой вершины, сохраняем путь
    if (current == target) {
        allRoutes.push_back(currentPath);
    } else {
        // пробуем все исходящие рёбра
        auto outgoing = graph.getOutgoingEdges(current);
        for (const auto& edge : outgoing) {
            int next = edge.first;
            // если вершина не посещена, идём в неё
            if (!visited[next]) {
                findAllPaths(next, target);
            }
        }
    }
    
    // откатываем изменения (backtracking)
    visited[current] = false;
    currentPath.pop_back();
}

/*
	LOOK: vector<vector<int>> findAllRoutes(source, target)
	Надстройка над findAllPath с валидацией параметров и очисткой предыдущих результатов
*/
template <typename T>
vector<vector<int>> RouteCounter<T>::findAllRoutes(int source, int target) {
    // очищаем предыдущие результаты
    allRoutes.clear();
    currentPath.clear();
    fill(visited.begin(), visited.end(), false);
    
    // проверяем корректность вершин
    if (source < 0 || source >= n || target < 0 || target >= n) {
        return allRoutes;
    }
    
    // запускаем поиск
    findAllPaths(source, target);
    
    return allRoutes;
}

/*
	LOOK: void printRoutes(source, target, vector<vector<int>>& routes) 
	Выводим все найденные маршруты
*/
template <typename T>
void RouteCounter<T>::printRoutes(int source, int target, const vector<vector<int>>& routes) {
    cout << "\n=== поиск маршрутов от вершины " << source << " до вершины " << target << " ===\n\n";
    
    if (routes.empty()) {
        cout << "существует маршрут: НЕТ\n";
        cout << "количество маршрутов: 0\n";
        return;
    }
    
    cout << "существует маршрут: ДА\n";
    cout << "количество маршрутов: " << routes.size() << "\n\n";
    cout << "все найденные маршруты:\n";
    
    for (size_t i = 0; i < routes.size(); i++) {
        cout << "  маршрут " << (i + 1) << ": ";
        for (size_t j = 0; j < routes[i].size(); j++) {
            cout << routes[i][j];
            if (j < routes[i].size() - 1) {
                cout << " -> ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}