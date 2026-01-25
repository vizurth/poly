#ifndef UI_H
#define UI_H

#include "../../common/graph/graph.h"

// класс для управления пользовательским интерфейсом lab1
class UI {
private:
    Graph<double>* currentGraph;  // текущий сгенерированный граф
    
    // очистить буфер ввода
    void clearInput();
    
    // задание 1: генерация связного ациклического графа (DAG)
    void task1_generateDAG();
    
    // задание 2: метод Шимбелла
    void task2_shimbel();
    
    // задание 3: подсчёт маршрутов методом backtracking
    void task3_backtracking();
    
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
