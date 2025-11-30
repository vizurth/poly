#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

// класс для построения полинома Жигалкина
class ZhegalkinPolynomial {
private:
    int numVars; // кол-во переменных
    vector<int> truthTable; // таблица истинности
    vector<vector<int>> triangle; // треугольник для метода Паскаля
    vector<string> minterms; // термы полинома
    
public:
    ZhegalkinPolynomial(int n) : numVars(n) {
        int size = pow(2, n);
        truthTable.resize(size);
        triangle.resize(size);
        for (int i = 0; i < size; i++) {
            triangle[i].resize(size - i);
        }
    }
    
    // создание таблицы истинности
    void setTruthTableFromVector(const vector<int>& F) {
        int size = pow(2, numVars);
        
        if (F.size() != size) {
            cerr << "Ошибка! Размер вектора F (" << F.size() 
                 << ") не соответствует количеству переменных (ожидается " 
                 << size << ")" << endl;
            return;
        }
        
        truthTable = F;
        
        cout << "\n=== Таблица истинности загружена ===" << endl;
        // Печать заголовка
        for (int i = 0; i < numVars; i++) {
            cout << "x" << (i + 1) << " ";
        }
        cout << "| f" << endl;
        cout << string(numVars * 3 + 3, '-') << endl;
        
        // Вывод значений
        for (int i = 0; i < size; i++) {
            for (int j = numVars - 1; j >= 0; j--) {
                cout << ((i >> j) & 1) << "  ";
            }
            cout << "| " << truthTable[i] << endl;
        }
        cout << endl;
    }
    
    // Построение треугольника (метод треугольника Паскаля)
    void buildTriangle() {
        int size = pow(2, numVars);
        
        for (int i = 0; i < size; i++) {
            triangle[0][i] = truthTable[i];
        }
        
        for (int row = 1; row < size; row++) {
            for (int col = 0; col < size - row; col++) {
                triangle[row][col] = triangle[row - 1][col] ^ triangle[row - 1][col + 1];
            }
        }
    }
    
    // Отображение треугольника
    void printTriangle() {
        int size = pow(2, numVars);
        cout << "\n=== Треугольник (метод Паскаля) ===" << endl;
        cout << "Каждая строка получается применением XOR к соседним элементам\n" << endl;
        
        for (int row = 0; row < size; row++) {
            cout << string(row * 2, ' ');
            
            for (int col = 0; col < size - row; col++) {
                cout << triangle[row][col] << "   ";
            }
            cout << endl;
        }
        
        cout << "\nЛевая колонка треугольника - коэффициенты полинома Жигалкина!" << endl;
    }
    
    // Генерация имени терма по индексу
    string generateMinterm(int index) {
        if (index == 0) return "1";
        
        string term = "";
        for (int i = 0; i < numVars; i++) {
            if ((index >> (numVars - 1 - i)) & 1) {
                if (term.length() > 0) term += "*";
                term += "x" + to_string(i + 1);
            }
        }
        return term;
    }
    
    // Построение полинома Жигалкина
    string buildPolynomial() {
        int size = pow(2, numVars);
        string polynomial = "";
        bool first = true;
        
        minterms.clear();
        
        // Левая колонка треугольника содержит коэффициенты
        for (int i = 0; i < size; i++) {
            if (triangle[i][0] == 1) {
                string term = generateMinterm(i);
                minterms.push_back(term);
                
                if (!first) {
                    polynomial += " ⊕ ";
                }
                polynomial += term;
                first = false;
            }
        }
        
        if (polynomial.empty()) {
            polynomial = "0";
        }
        
        return polynomial;
    }
    
    // Вывод результата
    void printResult() {
        string poly = buildPolynomial();
        
        cout << "\n=== Полином Жигалкина ===" << endl;
        cout << "f(";
        for (int i = 0; i < numVars; i++) {
            cout << "x" << (i + 1);
            if (i < numVars - 1) cout << ", ";
        }
        cout << ") = " << poly << endl;
        
        cout << "\n=== Пояснение ===" << endl;
        cout << "Коэффициенты из левой колонки треугольника:" << endl;
        int size = pow(2, numVars);
        for (int i = 0; i < size; i++) {
            cout << "  a" << i << " = " << triangle[i][0];
            if (triangle[i][0] == 1) {
                cout << " → включаем терм: " << generateMinterm(i);
            }
            cout << endl;
        }
    }
    
    // Вычисление значения полинома для заданного набора переменных
    int evaluatePolynomial(const vector<int>& vars) {
        if (vars.size() != numVars) {
            cerr << "Ошибка! Неверное количество переменных." << endl;
            return -1;
        }
        
        int size = pow(2, numVars);
        int result = 0;
        
        // Вычисляем каждый терм полинома
        for (int j = 0; j < size; j++) {
            if (triangle[j][0] == 1) {  // Если коэффициент = 1
                int termValue = 1;
                
                // Вычисляем значение терма (произведение переменных)
                for (int k = 0; k < numVars; k++) {
                    if ((j >> (numVars - 1 - k)) & 1) {
                        termValue &= vars[k];
                    }
                }
                
                // XOR с результатом
                result ^= termValue;
            }
        }
        
        return result;
    }
    
    // Интерактивное вычисление значения функции
    void interactiveEvaluation() {
        cout << "\n=== Вычисление значения функции ===" << endl;
        
        bool continueEval = true;
        while (continueEval) {
            vector<int> vars(numVars);
            
            cout << "\nВведите значения переменных (0 или 1):" << endl;
            for (int i = 0; i < numVars; i++) {
                int val;
                do {
                    cout << "x" << (i + 1) << " = ";
                    cin >> val;
                    if (val != 0 && val != 1) {
                        cout << "Ошибка! Введите 0 или 1." << endl;
                    }
                } while (val != 0 && val != 1);
                vars[i] = val;
            }
            
            // Вычисляем значение
            int result = evaluatePolynomial(vars);
            
            // Вывод результата
            cout << "\n--- Результат ---" << endl;
            cout << "f(";
            for (int i = 0; i < numVars; i++) {
                cout << vars[i];
                if (i < numVars - 1) cout << ", ";
            }
            cout << ") = " << result << endl;
            
            // Продолжить?
            char choice;
            cout << "\nВычислить для других значений? (y/n): ";
            cin >> choice;
            
            if (choice != 'y' && choice != 'Y') {
                continueEval = false;
            }
        }
    }
};