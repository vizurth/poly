#include "../include/calc.hpp"
#include "../include/config.hpp"
#include "../include/UI.hpp"

int main() {
    FiniteArithmeticCalc calc(N, plusOne, alph);
    
    UI ui(calc);
    ui.run();
    
    return 0;
}