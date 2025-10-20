#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>

using namespace std;

class FiniteArithmeticCalc {
private:
    // Основные параметры арифметики
    int N;
    map<string, string> plusOneRule;
    vector<string> alphabet;
    
    // Вычисленные структуры
    vector<string> hasseVector;
    map<string, int> hasseWeight;
    map<string, string> inverseMap;
    
    // Таблицы операций
    vector<vector<string>> addTable;
    vector<vector<string>> mulTable;
    vector<vector<string>> subTable;
    vector<vector<string>> divTable;
    
    // Специальные элементы
    string additiveIdentity;      // "a" (0)
    string multiplicativeIdentity; // "b" (1)
    string universum;             // "универсум" для 0*0
    string emptySet;              // "∅" для деления на 0
    
    // Вспомогательные функции инициализации
    void buildHasseVector();
    void buildHasseWeight();
    optional<string> findMultiplicativeInverse(const string& x);
    void buildInverseMap();
    void buildAddTable();
    void buildMulTable();
    void buildSubTable();
    void buildDivTable();
    
    // Вспомогательная функция для получения индекса элемента
    int getIndex(const string& elem) const;
    
    // Вспомогательная функция для вывода таблицы
    void printTable(const vector<vector<string>>& table) const;
    
    // Валидация элемента
    bool isValidElement(const string& elem) const;

public:
    // Конструктор
    FiniteArithmeticCalc(int n, 
                        const map<string, string>& rule, 
                        const vector<string>& alph,
                        const string& addId = "a",
                        const string& mulId = "b");
    
    // Геттеры для структур
    const vector<string>& getHasseVector() const;
    const map<string, int>& getHasseWeight() const;
    const map<string, string>& getInverseMap() const;
    const vector<vector<string>>& getAddTable() const;
    const vector<vector<string>>& getMulTable() const;
    const vector<vector<string>>& getSubTable() const;
    const vector<vector<string>>& getDivTable() const;
    const vector<string>& getAlphabet() const;
    
    // Операции калькулятора
    string add(const string& a, const string& b) const;
    string multiply(const string& a, const string& b) const;
    string subtract(const string& a, const string& b) const;
    string divide(const string& a, const string& b) const;
    
    // Проверка и получение обратного элемента
    bool isInvertible(const string& elem) const;
    optional<string> getInverse(const string& elem) const;
    
    // Вывод таблиц и информации
    void printAddTable() const;
    void printMulTable() const;
    void printSubTable() const;
    void printDivTable() const;
    void printAllTables() const;
    void printInfo() const;
    void printHelp() const;
};
