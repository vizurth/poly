#pragma once

#include "common/graph/graph.h"
#include "common/graph/dag_generator.h"
#include "common/distributions/weibull.h"
#include "lab1/include/shimbel.h"
#include "lab1/include/route_counter.h"
#include <memory>

using namespace std;

class UI {
private:
    unique_ptr<Graph<double>> graph;
    bool graphGenerated; // флаг генерации графа
    
    void printMenu();
    void task1_generateDAG();
    void task2_shimbel();
    void task3_routeCounter();
    
public:
    UI();
    void run();
};