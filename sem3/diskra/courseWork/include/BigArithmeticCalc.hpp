#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <tuple>

using namespace std;

// калькулятор большой конечной арифметики (8 разрядов)
// работает на основе малой арифметики с правилом "+1"
class BigArithmeticCalc {
private:
    // основные параметры арифметики
    int N;
    map<string, string> plusOneRule;
    vector<string> alphabet;
    
    // вычисленные структуры
    map<string, string> inverseMap;
    
    // таблицы для больших операций (с переносами)
    map<tuple<string, string, string>, pair<string, string>> additionTableWithCarry;
    map<string, string> negationMap;
    
    // таблицы операций (для малой арифметики)
    vector<vector<string>> addTable;
    vector<vector<string>> mulTable;
    vector<vector<string>> subTable;
    vector<vector<string>> divTable;
    
    // специальные элементы
    string additiveIdentity;
    string multiplicativeIdentity;
    string universum;
    string emptySet;
    
    // максимальная длина числа в разрядах
    static const int MAX_DIGITS = 8;
    
    // ============ малая арифметика ============
    
    // базовые операции через диаграмму хассе
    string addByHasse(const string& a, const string& b) const;
    string multiplyByHasse(const string& a, const string& b) const;
    string subtractByHasse(const string& a, const string& b) const;
    string divideByHasse(const string& a, const string& b) const;
    
    // поиск обратных элементов
    optional<string> findMultiplicativeInverse(const string& x);
    void buildInverseMap();
    
    // построение таблиц малой арифметики
    void buildAddTable();
    void buildMulTable();
    void buildSubTable();
    void buildDivTable();
    
    // построение таблиц для больших операций
    void buildAdditionTableWithCarry();
	bool checkCarry(const string& start, const string& steps) const;
    void buildNegationMap();
    
    // вспомогательные функции для работы с символами
    string nextSymbol(const string& current) const;
    int compareSymbols(const string& a, const string& b) const;
    
    // ============ большая арифметика ============
    
    // работа со знаками
    bool isNegative(const string& num) const;
    string removeSign(const string& num) const;
    string addSign(const string& num, bool negative) const;
    
    // проверка валидности
    bool isValidNumber(const string& num) const;
    bool isValidElement(const string& elem) const;
    
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
    
    // вспомогательные для вывода
    void printTable(const vector<vector<string>>& table) const;
    
public:
    // конструктор
    BigArithmeticCalc(int n, 
                     const map<string, string>& rule, 
                     const vector<string>& alph,
                     const string& addId = "0",
                     const string& mulId = "1");
    
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