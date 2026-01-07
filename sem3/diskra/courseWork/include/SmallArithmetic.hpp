#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <tuple>

using namespace std;

// Малая конечная арифметика на алфавите с правилом "+1"
class SmallArithmetic {
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

    // максимальная длина числа в разрядах (используется для построения универсума)
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

    // вспомогательные для вывода
    void printTable(const vector<vector<string>>& table) const;

public:
    SmallArithmetic(int n,
                    const map<string, string>& rule,
                    const vector<string>& alph,
                    const string& addId = "a",
                    const string& mulId = "b");

    // доступ к параметрам
    const vector<string>& getAlphabet() const;
    int getN() const;

    const string& getAdditiveIdentity() const;
    const string& getMultiplicativeIdentity() const;
    const string& getUniversum() const;
    const string& getEmptySet() const;
    int getMaxDigits() const;
    string getMinNumber() const;
    string getMaxNumber() const;

    // операции и утилиты, используемые большой арифметикой
    bool isValidElement(const string& elem) const;
    int compareElems(const string& a, const string& b) const;
    string nextElem(const string& current) const;
    pair<string, string> addWithCarry(const string& c1,
                                      const string& c2,
                                      const string& carry_in) const;
    string smallSubtract(const string& a, const string& b) const;

    // вывод таблиц и диаграммы
    void printAddTable() const;
    void printMulTable() const;
    void printSubTable() const;
    void printDivTable() const;
    void printAllTables() const;
    void printHasseDiagram() const;
};


