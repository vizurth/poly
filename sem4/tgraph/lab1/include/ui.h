#ifndef UI_H
#define UI_H

#include "common/graph/graph.h"
#include "common/graph/dag_generator.h"
#include "common/distributions/normal.h"
#include "lab1/include/shimbel.h"
#include "lab1/include/route_counter.h"
#include <memory>

class UI {
private:
    std::unique_ptr<Graph<double>> graph; // текущий граф
    bool graphGenerated; // флаг что граф создан
    
    void printMenu();
    void task1_generateDAG();
    void task2_shimbel();
    void task3_routeCounter();
    
public:
    UI();
    void run(); // главный цикл
};

#endif // UI_H
