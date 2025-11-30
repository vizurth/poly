// menu_system.h
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "bdd.h"
#include "help.h"
#include "zheg.h"
#include "config.h"

using namespace std;

class UI {
private:
    vector<string> vars;
    vector<int> F;
    bool running;
    
    void clearScreen() {
        system("clear");
    }
    
    void pause() {
        cout << "\nнажмите любую кнопку для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    void printHeader() {
        cout << "===== Лабораторная работа по Дискретной Математике №2 =====" << endl;
    }
    
    void displayCurrentFunction() {
        int numVars = vars.size();
        
        // переменные в заголовке
        for (int i = 0; i < numVars; i++) {
            cout << vars[i] << " ";
        }
        cout << "| f" << endl;
        cout << string(numVars * 3 + 3, '-') << endl;
        
        // значения переменных и функции
        for (size_t i = 0; i < F.size(); i++) {
            for (int j = numVars - 1; j >= 0; j--) {
                cout << ((i >> j) & 1) << "  ";
            }
            cout << "| " << F[i] << endl;
        }
        cout << endl;
    }
    
    int showMenu() {
        cout << "===== ГЛАВНОЕ МЕНЮ =====" << endl;
		cout << "1. показать текущую функцию" << endl;
		cout << "2. построить СДНФ и СКНФ" << endl;
		cout << "3. работа с BDD (Binary Decision Diagram)" << endl;
		cout << "4. построить полином Жигалкина" << endl;
		cout << "0. выход" << endl;
        
        int choice;
        cout << "\nВыберите опцию: ";
        cin >> choice;
        return choice;
    }
    
    void showCurrentFunction() {
        clearScreen();
        cout << "===== ТЕКУЩАЯ КОНФИГУРАЦИЯ БУЛЕВОЙ ФУНКЦИИ =====" << endl;
        
        displayCurrentFunction();
        
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
    
    void buildNormalForms() {
        clearScreen();
        cout << "======= ПОСТРОЕНИЕ НОРМАЛЬНЫХ ФОРМ =======" << endl;
        
        displayCurrentFunction();
        
        cout << "=== РЕЗУЛЬТАТЫ ===" << endl;
        cout << "\nСДНФ:" << endl;
        cout << buildSDNF(vars, F) << endl;
        
        cout << "\nСКНФ:" << endl;
        cout << buildSKNF(vars, F) << endl;
        
        pause();
    }
    
    void workWithBDD() {
        clearScreen();
        cout << "====== Работа с BDD (Binary Decision Diagram) ======" << endl;
        
        displayCurrentFunction();
        
        // создаем и инициализируем BDD граф
        BDDGraph bdd;
        bdd.buildFromDiagram();
        
        // выводим структуру графа
        bdd.printGraph();
        
        bool continueCalculations = true;
        while (continueCalculations) {
            // переменные для хранения значений
            map<string, int> values;
            
            cout << "\n=== Вычисление по BDD ===" << endl;
            cout << "Введите значения переменных (0 или 1):" << endl;
            
            // ввод значений переменных
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
            
            // вычисляем результат
            vector<string> path;
            int result = bdd.evaluate(values, path);
            
            // выводим результат
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
            
            
            char choice;
            cout << "\nВычислить для других значений? (y/n): ";
            cin >> choice;
            
            if (choice != 'y' && choice != 'Y') {
                continueCalculations = false;
            }
        }
        
        pause();
    }
    
    void buildZhegalkinPolynomial() {
        clearScreen();
        cout << "====== ПОСТРОЕНИЕ ПОЛИНОМА ЖИГАЛКИНА(метод треугольника) ======" << endl;
        
        int numVars = vars.size();
        ZhegalkinPolynomial zp(numVars);
        
        // устанавливаем таблицу истинности из конфига
        zp.setTruthTableFromVector(F);
        
        // построение треугольника
        zp.buildTriangle();
        
        // вывод треугольника
        zp.printTriangle();
        
        // построение и вывод полинома
        zp.printResult();
        
        // интерактивное вычисление
        char choice;
        cout << "\nВычислить значение для конкретных значений переменных? (y/n): ";
        cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            zp.interactiveEvaluation();
        }
        
        pause();
    }
    
    
    void handleMenuChoice(int choice) {
        switch (choice) {
            case 1:
                showCurrentFunction();
                break;
            
            case 2:
                buildNormalForms();
                break;
            
            case 3:
                workWithBDD();
                break;
            
            case 4:
                buildZhegalkinPolynomial();
                break;
            
            case 0:
                cout << "\nВыход из программы. До свидания!" << endl;
                running = false;
                break;
            
            default:
                cout << "\nОшибка! Неверный выбор. Попробуйте снова." << endl;
                pause();
                break;
        }
    }
    
public:
    UI(const vector<string>& variables, const vector<int>& function)
        : vars(variables), F(function), running(true) {}
    
    void run() {
        while (running) {
            clearScreen();
            printHeader();
            
            int choice = showMenu();
            handleMenuChoice(choice);
        }
    }
};