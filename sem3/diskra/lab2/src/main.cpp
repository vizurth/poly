#include <iostream>
#include <vector>
#include <string>

#include "../include/bdd.h"
#include "../include/help.h"
#include "../include/zheg.h"
#include "../include/config.h"
#include "../include/ui.h"

using namespace std;

int main() {
    // Создаём систему меню с конфигурацией из config.h
    UI menu(vars, F);
    
    // Запускаем главный цикл
    menu.run();
    
    return 0;
}