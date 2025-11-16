#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

using namespace std;

/**
 * Калькулятор "большой" конечной арифметики (8 разрядов)
 * Работает на основе "малой" арифметики с правилом "+1"
 */
class BigArithmeticCalc {
private:
    // Основные параметры арифметики
    int N;
    map<string, string> plusOneRule;
    vector<string> alphabet;
    
    // Вычисленные структуры
    map<string, string> inverseMap;
    
    // Таблицы операций (для малой арифметики)
    vector<vector<string>> addTable;
    vector<vector<string>> mulTable;
    vector<vector<string>> subTable;
    vector<vector<string>> divTable;
    
    // Специальные элементы
    string additiveIdentity;      // "a" (0)
    string multiplicativeIdentity; // "b" (1)
    string universum;             // "универсум" для a*a
    string emptySet;              // "∅" для деления на a
    
    // ============ МАЛАЯ АРИФМЕТИКА (1 разряд) ============
    
    // Применить plusOne N раз
    string applyPlusOneNTimes(const string& start, int n) const;
    
    // Базовые операции через диаграмму Хассе
    string addByHasse(const string& a, const string& b) const;
    string multiplyByHasse(const string& a, const string& b) const;
    string subtractByHasse(const string& a, const string& b) const;
    string divideByHasse(const string& a, const string& b) const;
    
    // Поиск обратных элементов
    optional<string> findMultiplicativeInverse(const string& x);
    void buildInverseMap();
    
    // Построение таблиц малой арифметики
    void buildAddTable();
    void buildMulTable();
    void buildSubTable();
    void buildDivTable();
    
    // ============ БОЛЬШАЯ АРИФМЕТИКА (8 разрядов) ============
    
    // Проверка: является ли строка валидным числом
    bool isValidNumber(const string& num) const;
    
    // Нормализация: удаление ведущих нулей
    string normalize(const string& num) const;
    
    // Сложение больших чисел (столбиком с переносом)
    string addBig(const string& a, const string& b) const;
    
    // Вычитание больших чисел (столбиком с займом)
    string subtractBig(const string& a, const string& b) const;
    
    // Умножение больших чисел (столбиком)
    string multiplyBig(const string& a, const string& b) const;
    
    // Деление больших чисел (столбиком)
    pair<string, string> divideBig(const string& a, const string& b) const;
    
    // Умножение большого числа на однозначное
    string multiplyByDigit(const string& num, const string& digit) const;
    
    // Сравнение больших чисел
    int compareBig(const string& a, const string& b) const;
    
    // Сдвиги (умножение/деление на "основание")
    string shiftLeft(const string& num, int positions) const;
    string shiftRight(const string& num, int positions) const;
    
    // ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============
    
    int getIndex(const string& elem) const;
    bool isValidElement(const string& elem) const;
    void printTable(const vector<vector<string>>& table) const;
    
public:
    // Конструктор
    BigArithmeticCalc(int n, 
                     const map<string, string>& rule, 
                     const vector<string>& alph,
                     const string& addId = "a",
                     const string& mulId = "b");
    
    // Геттеры
    const map<string, string>& getInverseMap() const;
    const vector<vector<string>>& getAddTable() const;
    const vector<vector<string>>& getMulTable() const;
    const vector<vector<string>>& getSubTable() const;
    const vector<vector<string>>& getDivTable() const;
    const vector<string>& getAlphabet() const;
    
    // ============ ОПЕРАЦИИ МАЛОЙ АРИФМЕТИКИ ============
    
    string addSmall(const string& a, const string& b) const;
    string multiplySmall(const string& a, const string& b) const;
    string subtractSmall(const string& a, const string& b) const;
    string divideSmall(const string& a, const string& b) const;
    
    // ============ ОПЕРАЦИИ БОЛЬШОЙ АРИФМЕТИКИ (8 разрядов) ============
    
    string add(const string& a, const string& b) const;
    string multiply(const string& a, const string& b) const;
    string subtract(const string& a, const string& b) const;
    string divide(const string& a, const string& b) const; // возвращает "Q|R"
    
    // ============ ПРОВЕРКИ И УТИЛИТЫ ============
    
    bool isInvertible(const string& elem) const;
    optional<string> getInverse(const string& elem) const;
    
    // Вывод таблиц и информации
    void printAddTable() const;
    void printMulTable() const;
    void printSubTable() const;
    void printDivTable() const;
    void printAllTables() const;
    void printInfo() const;
    void printHasseDiagram() const;
    void printInvertibleElements() const;
    void printHelp() const;
};