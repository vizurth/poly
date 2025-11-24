#include "../include/UI.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>

using namespace std;

// Конструктор
UI::UI(BigArithmeticCalc& calculator) : calc(calculator) {}

// Вспомогательные функции
string UI::trim(const string& str) const {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

string UI::toLower(const string& str) const {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Отображение интерфейса

void UI::displayWelcome() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║  Калькулятор БОЛЬШОЙ конечной арифметики Z" << calc.getAlphabet().size() << "            ║" << endl;
    cout << "║  Поддержка чисел до 8 разрядов                         ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    cout << "\nВведите 'help' для справки по командам" << endl;
    cout << "Введите 'info' для информации о системе\n" << endl;
}

void UI::displayPrompt() const {
    cout << "calc> ";
}

void UI::displayResult(const string& expression, const string& result) const {
    cout << "  ➜  " << expression << " = " << result << endl;
}

void UI::displayError(const string& message) const {
    cout << "❌ Ошибка: " << message << endl;
}

void UI::displayGoodbye() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║  Спасибо за использование калькулятора!                ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝\n" << endl;
}

// Проверка команды выхода
bool UI::isExitCommand(const string& command) const {
    return command == "exit" || command == "quit" || command == "q";
}

// Обработчики команд
void UI::handleHelp() const {
    calc.printHelp();
}

void UI::handleInfo() const {
    calc.printInfo();
}

void UI::handleHasse() const {
    calc.printHasseDiagram();
}

void UI::handleInvList() const {
    calc.printInvertibleElements();
}

void UI::handleTables() const {
    calc.printAllTables();
}

void UI::handleAddTable() const {
    calc.printAddTable();
}

void UI::handleMulTable() const {
    calc.printMulTable();
}

void UI::handleSubTable() const {
    calc.printSubTable();
}

void UI::handleDivTable() const {
    calc.printDivTable();
}

void UI::handleInverse(const string& element) const {
    auto inv = calc.getInverse(element);
    if (inv.has_value()) {
        displayResult(element + "⁻¹", inv.value());
    } else {
        cout << "  ℹ️  Элемент '" << element << "' не имеет обратного элемента" << endl;
    }
}

void UI::handleBigOperation(const string& operand1, const string& operation, 
                           const string& operand2) const {
    string result;
    string op_symbol;
    
    if (operation == "+") {
        result = calc.add(operand1, operand2);
        op_symbol = "+";
    } else if (operation == "-") {
        result = calc.subtract(operand1, operand2);
        op_symbol = "-";
    } else if (operation == "*") {
        result = calc.multiply(operand1, operand2);
        op_symbol = "*";
    } else if (operation == "/") {
        result = calc.divide(operand1, operand2);
        op_symbol = "/";
    } else {
        displayError("неизвестная операция '" + operation + "' для больших чисел");
        return;
    }
    
    cout << "\n════ Результат большой арифметики ════" << endl;
    cout << "  " << operand1 << " " << op_symbol << " " << operand2 << " = " << result << endl;
	cout << "═══════════════════════════════════════\n" << endl;
}

// Обработка команд
bool UI::processCommand(const string& input) {
    string trimmedInput = trim(input);
    if (trimmedInput.empty()) return true;
    
    string command = toLower(trimmedInput);
    
    // Проверка команды выхода
    if (isExitCommand(command)) {
        return false;
    }
    
    // Команда помощи
    if (command == "help" || command == "h") {
        handleHelp();
        return true;
    }
    
    // Команда информации
    if (command == "info" || command == "i") {
        handleInfo();
        return true;
    }
    
    // Диаграмма Хассе
    if (command == "hasse" || command == "diagram") {
        handleHasse();
        return true;
    }
    
    // Список обратимых элементов
    if (command == "inv_list" || command == "invertible") {
        handleInvList();
        return true;
    }
    
    // Команды таблиц
    if (command == "tables" || command == "t") {
        handleTables();
        return true;
    }
    if (command == "add_table" || command == "at") {
        handleAddTable();
        return true;
    }
    if (command == "mul_table" || command == "mt") {
        handleMulTable();
        return true;
    }
    if (command == "sub_table" || command == "st") {
        handleSubTable();
        return true;
    }
    if (command == "div_table" || command == "dt") {
        handleDivTable();
        return true;
    }
    
    // Парсинг команд с параметрами
    istringstream iss(trimmedInput);
    string token1, operation, token2;
    
    iss >> token1;
    
    // Команда inv (обратный элемент)
    if (toLower(token1) == "inv") {
        iss >> token2;
        if (token2.empty()) {
            displayError("не указан элемент для поиска обратного");
            return true;
        }
        handleInverse(token2);
        return true;
    }
    
    // Операции
    iss >> operation >> token2;
    
    if (operation.empty() || token2.empty()) {
        displayError("неверный формат команды. Введите 'help' для справки");
        return true;
    }
    
    bool isBigOp = (operation == "+" || operation == "-" || 
                    operation == "*" || operation == "/");
    
    if (isBigOp) {
        // Большая арифметика
        handleBigOperation(token1, operation, token2);
    } else {
        displayError("неверный формат операции. Используйте '+', '-', '*', '/' для большой");
    }
    
    return true;
}

// Основной метод запуска
void UI::run() {
    displayWelcome();
    
    // Показываем основную информацию при запуске
    calc.printInfo();
    calc.printHasseDiagram();
    
    string input;
    bool running = true;
    
    while (running) {
        displayPrompt();
        getline(cin, input);
        
        running = processCommand(input);
    }
    
    displayGoodbye();
}