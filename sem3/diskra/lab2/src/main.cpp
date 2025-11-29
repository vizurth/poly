#include <iostream>
#include <vector>
#include <string>

#include "../include/bdd.h"
#include "../include/help.h"
#include "../include/zheg.h"
#include "../include/config.h"
using namespace std;

// x  – идентификаторы переменных: x[1..n]
// F  – значения функции: F[0..2^n - 1], где F[i] ∈ {0,1}
// результат — строка, представляющая СДНФ

// Функция для очистки экрана (кроссплатформенная)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Функция для паузы
void pause() {
    cout << "\nНажмите Enter для продолжения...";
    cin.ignore();
    cin.get();
}

// Вывод красивого заголовка
void printHeader() {
    cout << "╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║     СИСТЕМА АНАЛИЗА БУЛЕВЫХ ФУНКЦИЙ                  ║" << endl;
    cout << "║     Boolean Functions Analysis System                ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
}

// Отображение текущей функции
void displayCurrentFunction(const vector<string>& vars, const vector<int>& F) {
    cout << "\n┌────────────────────────────────────────┐" << endl;
    cout << "│   ТЕКУЩАЯ ФУНКЦИЯ ИЗ КОНФИГА          │" << endl;
    cout << "└────────────────────────────────────────┘" << endl;
    
    int numVars = vars.size();
    
    // Заголовок таблицы
    for (int i = 0; i < numVars; i++) {
        cout << vars[i] << " ";
    }
    cout << "| f" << endl;
    cout << string(numVars * 3 + 3, '-') << endl;
    
    // Значения
    for (size_t i = 0; i < F.size(); i++) {
        for (int j = numVars - 1; j >= 0; j--) {
            cout << ((i >> j) & 1) << "  ";
        }
        cout << "| " << F[i] << endl;
    }
    cout << endl;
}

// Вывод меню
int showMenu() {
    cout << "\n┌────────────────────────────────────────┐" << endl;
    cout << "│          ГЛАВНОЕ МЕНЮ                  │" << endl;
    cout << "├────────────────────────────────────────┤" << endl;
    cout << "│  1. Показать текущую функцию           │" << endl;
    cout << "│  2. Построить СДНФ и СКНФ              │" << endl;
    cout << "│  3. Работа с BDD (граф решений)        │" << endl;
    cout << "│  4. Полином Жигалкина                  │" << endl;
    cout << "│  5. Выполнить все операции             │" << endl;
    cout << "│  0. Выход                              │" << endl;
    cout << "└────────────────────────────────────────┘" << endl;
    
    int choice;
    cout << "\nВыберите опцию: ";
    cin >> choice;
    return choice;
}

// Опция 1: Показать текущую функцию
void showCurrentFunction(const vector<string>& vars, const vector<int>& F) {
    clearScreen();
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  ТЕКУЩАЯ КОНФИГУРАЦИЯ                     ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    
    displayCurrentFunction(vars, F);
    
    cout << "Переменные: ";
    for (size_t i = 0; i < vars.size(); i++) {
        cout << vars[i];
        if (i < vars.size() - 1) cout << ", ";
    }
    cout << endl;
    
    cout << "Вектор функции: [";
    for (size_t i = 0; i < F.size(); i++) {
        cout << F[i];
        if (i < F.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    pause();
}

// Опция 2: СДНФ и СКНФ
void buildNormalForms(const vector<string>& vars, const vector<int>& F) {
    clearScreen();
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  ПОСТРОЕНИЕ СДНФ И СКНФ                   ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    
    displayCurrentFunction(vars, F);
    
    cout << "=== РЕЗУЛЬТАТЫ ===" << endl;
    cout << "\nСДНФ (Совершенная дизъюнктивная нормальная форма):" << endl;
    cout << buildSDNF(vars, F) << endl;
    
    cout << "\nСКНФ (Совершенная конъюнктивная нормальная форма):" << endl;
    cout << buildSKNF(vars, F) << endl;
    
    pause();
}

// Опция 3: Работа с BDD
void workWithBDD(const vector<string>& vars, const vector<int>& F) {
    clearScreen();
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  РАБОТА С BDD (Binary Decision Diagram)   ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    
    displayCurrentFunction(vars, F);
    
    // Создаем и инициализируем BDD граф
    BDDGraph bdd;
    bdd.buildFromDiagram();
    
    // Выводим структуру графа
    bdd.printGraph();
    
    bool continueCalculations = true;
    while (continueCalculations) {
        // Переменные для хранения значений
        map<string, int> values;
        
        cout << "\n=== Вычисление по BDD ===" << endl;
        cout << "Введите значения переменных (0 или 1):" << endl;
        
        // Ввод значений переменных
        for (size_t i = 0; i < vars.size(); i++) {
            int val;
            do {
                cout << vars[i] << " = ";
                cin >> val;
                if (val != 0 && val != 1) {
                    cout << "Ошибка! Введите 0 или 1." << endl;
                }
            } while (val != 0 && val != 1);
            values[vars[i]] = val;
        }
        
        // Вычисляем результат
        vector<string> path;
        int result = bdd.evaluate(values, path);
        
        // Выводим результат
        cout << "\n=== Результат вычисления ===" << endl;
        cout << "Входные значения: ";
        for (size_t i = 0; i < vars.size(); i++) {
            cout << vars[i] << "=" << values[vars[i]];
            if (i < vars.size() - 1) cout << ", ";
        }
        cout << endl;
        
        cout << "\nПуть по графу:" << endl;
        for (size_t i = 0; i < path.size(); i++) {
            cout << "  " << (i + 1) << ". " << path[i] << endl;
        }
        
        cout << "\nИтоговый результат: " << result << endl;
        
        // Проверяем соответствие с таблицей истинности
        int index = 0;
        for (int i = vars.size() - 1; i >= 0; i--) {
            index = (index << 1) | values[vars[i]];
        }
        
        cout << "\nПроверка: f(";
        for (size_t i = 0; i < vars.size(); i++) {
            cout << values[vars[i]];
            if (i < vars.size() - 1) cout << ",";
        }
        cout << ") = " << F[index];
        
        if (result == F[index]) {
            cout << " ✓ Совпадает!" << endl;
        } else {
            cout << " ✗ Не совпадает!" << endl;
        }
        
        // Спрашиваем о продолжении
        char choice;
        cout << "\nВычислить для других значений? (y/n): ";
        cin >> choice;
        
        if (choice != 'y' && choice != 'Y') {
            continueCalculations = false;
        }
    }
    
    pause();
}

// Опция 4: Полином Жигалкина
void buildZhegalkinPolynomial(const vector<string>& vars, const vector<int>& F) {
    clearScreen();
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  ПОСТРОЕНИЕ ПОЛИНОМА ЖИГАЛКИНА             ║" << endl;
    cout << "║  Метод треугольника (Паскаля)              ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    
    int numVars = vars.size();
    ZhegalkinPolynomial zp(numVars);
    
    // Устанавливаем таблицу истинности из конфига
    zp.setTruthTableFromVector(F);
    
    // Построение треугольника
    zp.buildTriangle();
    
    // Вывод треугольника
    zp.printTriangle();
    
    // Построение и вывод полинома
    zp.printResult();
    
    // Интерактивное вычисление
    char choice;
    cout << "\nВычислить значение для конкретных значений переменных? (y/n): ";
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        zp.interactiveEvaluation();
    }
    
    pause();
}
// Опция 5: Выполнить все операции
void performAllOperations(const vector<string>& vars, const vector<int>& F) {
    clearScreen();
    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║  ПОЛНЫЙ АНАЛИЗ БУЛЕВОЙ ФУНКЦИИ            ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    
    displayCurrentFunction(vars, F);
    
    // 1. СДНФ и СКНФ
    cout << "\n" << string(60, '=') << endl;
    cout << "1. НОРМАЛЬНЫЕ ФОРМЫ" << endl;
    cout << string(60, '=') << endl;
    cout << "\nСДНФ: " << buildSDNF(vars, F) << endl;
    cout << "\nСКНФ: " << buildSKNF(vars, F) << endl;
    
    // 2. Полином Жигалкина
    cout << "\n" << string(60, '=') << endl;
    cout << "2. ПОЛИНОМ ЖИГАЛКИНА" << endl;
    cout << string(60, '=') << endl;
    
    int numVars = vars.size();
    ZhegalkinPolynomial zp(numVars);
    zp.setTruthTableFromVector(F);
    zp.buildTriangle();
    zp.printTriangle();
    zp.printResult();
    
    // 3. BDD информация
    cout << "\n" << string(60, '=') << endl;
    cout << "3. BDD (BINARY DECISION DIAGRAM)" << endl;
    cout << string(60, '=') << endl;
    cout << "\nДля интерактивной работы с BDD выберите опцию 3 в меню." << endl;
    
    BDDGraph bdd;
    bdd.buildFromDiagram();
    bdd.printGraph();
    
    pause();
}

int main() {
    // Загружаем конфигурацию из config.h
    // vector<int> F и vector<string> vars уже определены в config.h
    
    bool running = true;
    
    while (running) {
        clearScreen();
        printHeader();
        
        int choice = showMenu();
        
        switch (choice) {
            case 1:
                showCurrentFunction(vars, F);
                break;
            
            case 2:
                buildNormalForms(vars, F);
                break;
            
            case 3:
                workWithBDD(vars, F);
                break;
            
            case 4:
                buildZhegalkinPolynomial(vars, F);
                break;
            
            case 5:
                performAllOperations(vars, F);
                break;
            
            case 0:
                cout << "\n╔════════════════════════════════════════════╗" << endl;
                cout << "║  Спасибо за использование программы!      ║" << endl;
                cout << "║  Thank you for using the program!         ║" << endl;
                cout << "╚════════════════════════════════════════════╝" << endl;
                running = false;
                break;
            
            default:
                cout << "\nОшибка! Неверный выбор. Попробуйте снова." << endl;
                pause();
                break;
        }
    }
    
    return 0;
}