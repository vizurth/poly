#pragma once

#include "BigArithmeticCalc.hpp"
#include <string>

using namespace std;

class UI {
private:
    BigArithmeticCalc& calc;
    
    // Вспомогательные функции
    string trim(const string& str) const;
    string toLower(const string& str) const;
    
    // Обработчики команд
    void handleHelp() const;
    void handleInfo() const;
    void handleHasse() const;
    void handleTables() const;
    void handleAddTable() const;
    void handleMulTable() const;
    void handleSubTable() const;
    void handleDivTable() const;
    void handleInverse(const string& element) const;
    void handleBigOperation(const string& operand1, const string& operation, 
                           const string& operand2) const;
    
    // Отображение интерфейса
    void displayWelcome() const;
    void displayPrompt() const;
    void displayResult(const string& expression, const string& result) const;
    void displayError(const string& message) const;
    void displayGoodbye() const;
    
    // Парсинг команд
    bool isExitCommand(const string& command) const;
    bool processCommand(const string& input);

public:
    // Конструктор
    UI(BigArithmeticCalc& calculator);
    
    // Основной метод запуска интерфейса
    void run();
};