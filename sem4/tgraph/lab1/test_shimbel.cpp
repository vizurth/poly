#include "lab1/include/shimbel.h"
#include "common/graph/graph.h"
#include <iostream>

int main() {
    std::cout << "=== SHIMBEL ALGORITHM TEST ===\n\n";
    
    // Test 1: Small manual graph
    std::cout << "Test 1: Manual 4-vertex DAG\n";
    std::cout << "Graph structure:\n";
    std::cout << "  0 -> 1 (weight 2.0)\n";
    std::cout << "  0 -> 2 (weight 5.0)\n";
    std::cout << "  1 -> 2 (weight 1.0)\n";
    std::cout << "  1 -> 3 (weight 4.0)\n";
    std::cout << "  2 -> 3 (weight 2.0)\n\n";
    
    Graph<double> graph1(4);
    graph1.addEdge(0, 1, 2.0);
    graph1.addEdge(0, 2, 5.0);
    graph1.addEdge(1, 2, 1.0);
    graph1.addEdge(1, 3, 4.0);
    graph1.addEdge(2, 3, 2.0);
    
    std::cout << "Adjacency Matrix:\n";
    graph1.printAdjMatrix();
    
    ShimbelSolver<double> solver1(graph1);
    
    // K=1: Direct edges
    std::cout << "\n--- K=1 (Direct edges) ---\n";
    auto shortest1 = solver1.computeShortestPaths(1);
    auto longest1 = solver1.computeLongestPaths(1);
    ShimbelSolver<double>::printMatrix(shortest1, "Матрица Шимбелла (мин) k = 1:");
    ShimbelSolver<double>::printMatrix(longest1, "Матрица Шимбелла (макс) k = 1:");
    
    // K=2: 2-edge paths
    std::cout << "\n--- K=2 (2-edge paths) ---\n";
    auto shortest2 = solver1.computeShortestPaths(2);
    auto longest2 = solver1.computeLongestPaths(2);
    ShimbelSolver<double>::printMatrix(shortest2, "Матрица Шимбелла (мин) k = 2:");
    ShimbelSolver<double>::printMatrix(longest2, "Матрица Шимбелла (макс) k = 2:");
    
    std::cout << "Expected for K=2:\n";
    std::cout << "  0->2: min=3.0 (0->1->2), max=3.0\n";
    std::cout << "  0->3: min=6.0 (0->1->3), max=7.0 (0->2->3)\n";
    std::cout << "  1->3: min=3.0 (1->2->3), max=3.0\n\n";
    
    // K=3: 3-edge paths
    std::cout << "\n--- K=3 (3-edge paths) ---\n";
    auto shortest3 = solver1.computeShortestPaths(3);
    auto longest3 = solver1.computeLongestPaths(3);
    ShimbelSolver<double>::printMatrix(shortest3, "Матрица Шимбелла (мин) k = 3:");
    ShimbelSolver<double>::printMatrix(longest3, "Матрица Шимбелла (макс) k = 3:");
    
    std::cout << "Expected for K=3:\n";
    std::cout << "  0->3: min=5.0 (0->1->2->3), max=5.0\n\n";
    
    // Test 2: Verify no cycles (DAG property)
    std::cout << "\n=== Test 2: DAG verification ===\n";
    std::cout << "Checking diagonal elements (i->i paths):\n";
    std::cout << "All diagonal elements should be 0 for K=1 or INF for K>1\n";
    
    bool dagValid = true;
    for (int i = 0; i < 4; i++) {
        if (shortest2[i][i] < std::numeric_limits<double>::max() / 2 && shortest2[i][i] > 0) {
            std::cout << "ERROR: Found cycle at vertex " << i << "\n";
            dagValid = false;
        }
    }
    
    if (dagValid) {
        std::cout << "✓ DAG property verified: no cycles detected\n";
    }
    
    // Test 3: Path analysis
    std::cout << "\n=== Test 3: Path analysis (0->3) ===\n";
    std::cout << "K=1: " << (shortest1[0][3] < std::numeric_limits<double>::max() / 2 ? 
                std::to_string(shortest1[0][3]) : "INF") << "\n";
    std::cout << "K=2: " << (shortest2[0][3] < std::numeric_limits<double>::max() / 2 ? 
                std::to_string(shortest2[0][3]) : "INF") << " (shortest), "
              << (longest2[0][3] < std::numeric_limits<double>::max() / 2 ? 
                std::to_string(longest2[0][3]) : "INF") << " (longest)\n";
    std::cout << "K=3: " << (shortest3[0][3] < std::numeric_limits<double>::max() / 2 ? 
                std::to_string(shortest3[0][3]) : "INF") << "\n";
    
    std::cout << "\n=== ALL TESTS COMPLETED ===\n";
    
    return 0;
}
