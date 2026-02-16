#include "common/graph/graph.h"
#include "common/graph/dag_generator.h"
#include "common/distributions/normal.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== тест базовых компонентов lab1 ===\n\n";
    
    // тест 1: создание графа вручную
    cout << "--- тест 1: создание графа вручную ---\n";
    Graph<double> g(4);
    g.addEdge(0, 1, 5.5);
    g.addEdge(0, 2, 3.2);
    g.addEdge(1, 2, 2.1);
    g.addEdge(1, 3, 4.7);
    g.addEdge(2, 3, 1.8);
    
    cout << "количество вершин: " << g.getNumVertices() << "\n";
    g.printAdjMatrix();
    g.printEdges();
    
    // тест 2: нормальное распределение
    cout << "\n--- тест 2: нормальное распределение ---\n";
    NormalDistribution dist(10.0, 3.0);
    cout << "параметры: μ=10.0, σ=3.0\n";
    cout << "10 случайных значений:\n";
    for (int i = 0; i < 10; i++) {
        cout << "  " << dist.generate() << "\n";
    }
    
    // тест 3: генерация DAG
    cout << "\n--- тест 3: генерация DAG ---\n";
    NormalDistribution dagDist(10.0, 3.0);
    DAGGenerator<double> generator(dagDist); // seed генерируется автоматически
    
    int numVertices = 10;
    cout << "генерация DAG с " << numVertices << " вершинами...\n";
    Graph<double> dag = generator.generateDAG(numVertices);

	cout << "сгенерированный DAG связный: " << (generator.isConnected(dag) ? "да" : "нет") << "\n";
    
    dag.printAdjMatrix();
    dag.printEdges();
    
    // тест 4: методы графа
    cout << "\n--- тест 4: методы графа ---\n";
    cout << "исходящие рёбра из вершины 0:\n";
    auto outgoing = dag.getOutgoingEdges(0);
    for (const auto& edge : outgoing) {
        cout << "  0 -> " << edge.first << " (вес: " << edge.second << ")\n";
    }
    
    cout << "\nвходящие рёбра в вершину 3:\n";
    auto incoming = dag.getIncomingEdges(3);
    for (const auto& edge : incoming) {
        cout << "  " << edge.first << " -> 3 (вес: " << edge.second << ")\n";
    }
    
    cout << "\nпроверка наличия рёбер:\n";
    cout << "  ребро 0->1 существует: " << (dag.hasEdge(0, 1) ? "да" : "нет") << "\n";
    cout << "  ребро 3->0 существует: " << (dag.hasEdge(3, 0) ? "да" : "нет") << "\n";
    
    cout << "\n=== все тесты завершены ===\n";
    
    return 0;
}
