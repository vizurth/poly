#include "../include/UI.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

// Конструктор
UI::UI(FiniteArithmeticCalc& calculator) : calc(calculator) {}

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
    cout << "║  Калькулятор конечной арифметики Z" << calc.getAlphabet().size() << "                    ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    cout << "\nВведите 'help' для справки\n" << endl;
}

void UI::displayPrompt() const {
    cout << "> ";
}

void UI::displayResult(const string& expression, const string& result) const {
    cout << "Результат: " << expression << " = " << result << endl;
}

void UI::displayError(const string& message) const {
    cout << "Ошибка: " << message << endl;
}

void UI::displayGoodbye() const {
    cout << "\nВыход из программы. До свидания!" << endl;
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
        displayResult(element + "^-1", inv.value());
    } else {
        cout << "Элемент " << element << " не имеет обратного элемента" << endl;
    }
}

void UI::handleBinaryOperation(const string& operand1, const string& operation, const string& operand2) const {
    string result;
    
    if (operation == "+") {
        result = calc.add(operand1, operand2);
    } else if (operation == "-") {
        result = calc.subtract(operand1, operand2);
    } else if (operation == "*") {
        result = calc.multiply(operand1, operand2);
    } else if (operation == "/") {
        result = calc.divide(operand1, operand2);
    } else {
        displayError("неизвестная операция '" + operation + "'");
        return;
    }
    
    displayResult(operand1 + " " + operation + " " + operand2, result);
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
    string token1, operation, token2, token3;
    
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
    
    // Обычные операции (бинарные и возведение в степень)
    iss >> operation >> token2;
    
    if (operation.empty() || token2.empty()) {
        displayError("неверный формат команды. Введите 'help' для справки");
        return true;
    }

    // Бинарные операции
    handleBinaryOperation(token1, operation, token2);
    return true;
}

// Основной метод запуска

void UI::run() {
    displayWelcome();
    
    string input;
    bool running = true;
    
    while (running) {
        displayPrompt();
        getline(cin, input);
        
        running = processCommand(input);
    }
    
    displayGoodbye();
}