#ifndef UI_H
#define UI_H

#include "../../common/graph/graph.h"

// класс для управления пользовательским интерфейсом lab5
class UI {
private:
    Graph<double>* currentGraph;  // текущий сгенерированный граф
    
    // очистить буфер ввода
    void clearInput();
    
    // задание 1: проверка графа на эйлеровость и гамильтоновость
    void task1_checkProperties();
    
    // задание 2: эйлеров граф и эйлеров цикл
    void task2_eulerianCircuit();
    
    // задание 3: гамильтонов граф и задача коммивояжера
    void task3_tsp();
    
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
