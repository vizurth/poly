#ifndef UI_H
#define UI_H

#include "../../common/graph/graph.h"

// класс для управления пользовательским интерфейсом lab3
class UI {
private:
    Graph<double>* currentGraph;      // текущий сгенерированный DAG
    Graph<int>* capacityGraph;        // матрица пропускных способностей
    Graph<double>* costGraph;         // матрица стоимостей
    int maxFlowValue;                 // максимальный поток
    
    // очистить буфер ввода
    void clearInput();
    
    // задание 1: генерация матриц пропускных способностей и стоимостей
    void task1_generateMatrices();
    
    // задание 2: нахождение максимального потока (алгоритм Форда-Фалкерсона)
    void task2_maxFlow();
    
    // задание 3: нахождение потока минимальной стоимости величиной 2/3 от максимального
    void task3_minCostFlow();
    
    // показать главное меню
    void showMenu();
    
public:
    // конструктор
    UI();
    
    // деструктор
    ~UI();
    
    // запустить главный цикл программы
    void run();
};

#endif // UI_H
