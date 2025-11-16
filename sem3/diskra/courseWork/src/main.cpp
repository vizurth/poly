#include "../include/BigArithmeticCalc.hpp"
#include "../include/config.hpp"
#include "../include/UI.hpp"

int main() {
    // Создаём калькулятор большой арифметики
    BigArithmeticCalc calc(N, plusOne, alph);
    
    // Запускаем интерфейс
    UI ui(calc);
    ui.run();
    
    return 0;
}