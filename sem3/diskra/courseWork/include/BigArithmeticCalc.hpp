#pragma once

#include <string>
#include <utility>

#include "SmallArithmetic.hpp"

using namespace std;

// Калькулятор БОЛЬШОЙ конечной арифметики.
// Внутри использует класс SmallArithmetic для посимвольной (малой) арифметики.
class BigArithmeticCalc {
private:
    // Малая арифметика и все таблицы лежат здесь
    SmallArithmetic small;

    // ============ большая арифметика ============
    
    // работа со знаками
    bool isNegative(const string& num) const;
    string removeSign(const string& num) const;
    string addSign(const string& num, bool negative) const;
    
    // проверка валидности
    bool isValidNumber(const string& num) const;
    
    // нормализация: удаление ведущих нулей
    string deleteTrashZeros(const string& num) const;
    
    // проверка на переполнение
    bool isOverflow(const string& num) const;

    // операции столбиком (только для положительных)
    string addBigUnsigned(const string& a, const string& b) const;
    string subtractBigUnsigned(const string& a, const string& b) const;
    string multiplyBigUnsigned(const string& a, const string& b) const;
    pair<string, string> divideBigUnsigned(const string& a, const string& b) const;
    
    // умножение большого числа на однозначное
    string multiplyByDigit(const string& num, const string& digit) const;
    
    // сравнение больших чисел (без знака)
    int compareBigUnsigned(const string& a, const string& b) const;
    
public:
    // конструктор
    BigArithmeticCalc(int n, 
                      const map<string, string>& rule, 
                      const vector<string>& alph,
                      const string& addId = "a",
                      const string& mulId = "b");
    
    // геттеры
    const vector<string>& getAlphabet() const;
    string getMinNumber() const;
    string getMaxNumber() const;
    
    // ============ операции большой арифметики ============
    
    string add(const string& a, const string& b) const;
    string multiply(const string& a, const string& b) const;
    string subtract(const string& a, const string& b) const;
    string divide(const string& a, const string& b) const; 
    
    // вывод таблиц и информации
    void printAddTable() const;
    void printMulTable() const;
    void printSubTable() const;
    void printDivTable() const;
    void printAllTables() const;
    void printInfo() const;
    void printHasseDiagram() const;
    void printHelp() const;
};