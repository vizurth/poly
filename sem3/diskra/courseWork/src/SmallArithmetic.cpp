#include "../include/SmallArithmetic.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>

using namespace std;

// следующий символ по правилу +1
string SmallArithmetic::nextSymbol(const string& current) const {
    auto it = plusOneRule.find(current);
    if (it == plusOneRule.end()) {
        throw runtime_error("invalid symbol: " + current);
    }
    return it->second;
}

// функция для сравнения символов
/*
    если a == b или символы не найдены => 0
    если a > b => 1
    ecли a < b => -1
    ходим по бесконечному циклу через правило +1
*/
int SmallArithmetic::compareSymbols(const string& a, const string& b) const {
    if (a == b) return 0;

    string current = additiveIdentity;
    while (true) {
        if (current == a) return -1;
        if (current == b) return 1;
        current = nextSymbol(current);
        if (current == additiveIdentity) break;
    }
    return 0;
}

SmallArithmetic::SmallArithmetic(int n,
                                 const map<string, string>& rule,
                                 const vector<string>& alph,
                                 const string& addId,
                                 const string& mulId)
    : N(n),
      plusOneRule(rule),
      alphabet(alph),
      additiveIdentity(addId),
      multiplicativeIdentity(mulId),
      universum(""),
      emptySet("∅")
{
    universum = "[" + getMinNumber() + "; " + getMaxNumber() + "]";

    buildNegationMap();
    buildAdditionTableWithCarry();
    buildInverseMap();
    buildAddTable();
    buildMulTable();
    buildSubTable();
    buildDivTable();
}

// ============ малая арифметика ============

string SmallArithmetic::addByHasse(const string& a, const string& b) const {
    if (b == additiveIdentity) return a;

    string counter = additiveIdentity;
    string result = a;

    while (counter != b) {
        result = nextSymbol(result);
        counter = nextSymbol(counter);
    }

    return result;
}

string SmallArithmetic::multiplyByHasse(const string& a, const string& b) const {
    // специальный случай: a * a = универсум
    if (a == additiveIdentity && b == additiveIdentity) {
        return universum;
    }

    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }

    string counter = additiveIdentity;
    string result = additiveIdentity;

    // буквенный счетчик
    while (counter != b) {
        result = addByHasse(result, a);
        counter = nextSymbol(counter);
    }

    return result;
}

string SmallArithmetic::subtractByHasse(const string& a, const string& b) const {
    for (const auto& candidate : alphabet) {
        if (addByHasse(b, candidate) == a) {
            return candidate;
        }
    }
    return "ERR: with subtract";
}

string SmallArithmetic::divideByHasse(const string& a, const string& b) const {
    if (b == additiveIdentity) {
        return emptySet;
    }

    if (inverseMap.find(b) == inverseMap.end()) {
        return "ERR";
    }

    string bInverse = inverseMap.at(b);
    return multiplyByHasse(a, bInverse);
}

optional<string> SmallArithmetic::findMultiplicativeInverse(const string& x) {
    if (x == additiveIdentity) return nullopt;

    for (const auto& candidate : alphabet) {
        string product = multiplyByHasse(x, candidate);
        if (product == multiplicativeIdentity) {
            return candidate;
        }
    }
    return nullopt;
}

void SmallArithmetic::buildInverseMap() {
    for (const auto& elem : alphabet) {
        auto inv = findMultiplicativeInverse(elem);
        if (inv.has_value()) {
            inverseMap[elem] = inv.value();
        }
    }
}

void SmallArithmetic::buildAddTable() {
    addTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            addTable[i][j] = addByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void SmallArithmetic::buildMulTable() {
    mulTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mulTable[i][j] = multiplyByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void SmallArithmetic::buildSubTable() {
    subTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            subTable[i][j] = subtractByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void SmallArithmetic::buildDivTable() {
    divTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            divTable[i][j] = divideByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void SmallArithmetic::buildNegationMap() {
    //ищем x + candidate = additiveIdentity
    for (const auto& elem : alphabet) {
        for (const auto& candidate : alphabet) {
            if (addByHasse(elem, candidate) == additiveIdentity) {
                negationMap[elem] = candidate;
                break;
            }
        }
    }
}

void SmallArithmetic::buildAdditionTableWithCarry() {
    for (const auto& c1 : alphabet) {
        for (const auto& c2 : alphabet) {
            for (const auto& carry_in : {additiveIdentity, multiplicativeIdentity}) {
                string sum1 = addByHasse(c1, c2); // сумма без переноса
                string final_sum = addByHasse(sum1, carry_in); // финальная сумма с учетом переноса

                bool carry1 = checkCarry(c1, c2); // проверяем перенос с1 + с2
                bool carry2 = checkCarry(sum1, carry_in); // проверяем перенос sum1 + carry_in

                string carry_out = (carry1 || carry2) ? multiplicativeIdentity : additiveIdentity; // выставляем перенос

                additionTableWithCarry[make_tuple(c1, c2, carry_in)] = make_pair(final_sum, carry_out);
            }
        }
    }
}

bool SmallArithmetic::checkCarry(const string& start, const string& steps) const {
    if (steps == additiveIdentity) return false;

    string temp = start;
    string counter = additiveIdentity;

    while (counter != steps) {
        string old_temp = temp;
        temp = nextSymbol(temp);
        counter = nextSymbol(counter);

        if (old_temp != additiveIdentity && temp == additiveIdentity) {
            return true; // перенос обнаружен
        }
    }
    return false;
}

// ======== геттеры и утилиты, используемые большой арифметикой ========

const vector<string>& SmallArithmetic::getAlphabet() const {
    return alphabet;
}

int SmallArithmetic::getN() const {
    return N;
}

const string& SmallArithmetic::getAdditiveIdentity() const {
    return additiveIdentity;
}

const string& SmallArithmetic::getMultiplicativeIdentity() const {
    return multiplicativeIdentity;
}

const string& SmallArithmetic::getUniversum() const {
    return universum;
}

const string& SmallArithmetic::getEmptySet() const {
    return emptySet;
}

int SmallArithmetic::getMaxDigits() const {
    return MAX_DIGITS;
}

string SmallArithmetic::getMinNumber() const {
    string result = "-";
    for (int i = 0; i < MAX_DIGITS; i++) {
        result += alphabet.back();
    }
    return result;
}

string SmallArithmetic::getMaxNumber() const {
    string result;
    for (int i = 0; i < MAX_DIGITS; i++) {
        result += alphabet.back();
    }
    return result;
}

bool SmallArithmetic::isValidElement(const string& elem) const {
    for (const auto& x : alphabet) {
        if (x == elem) return true;
    }
    return false;
}

int SmallArithmetic::compareElems(const string& a, const string& b) const {
    return compareSymbols(a, b);
}

string SmallArithmetic::nextElem(const string& current) const {
    return nextSymbol(current);
}

pair<string, string> SmallArithmetic::addWithCarry(const string& c1,
                                                   const string& c2,
                                                   const string& carry_in) const {
    auto key = make_tuple(c1, c2, carry_in);
    auto it = additionTableWithCarry.find(key);
    if (it == additionTableWithCarry.end()) {
        return {"ERR", "ERR"};
    }
    return it->second;
}

string SmallArithmetic::smallSubtract(const string& a, const string& b) const {
    return subtractByHasse(a, b);
}

// ============ вывод таблиц ============

void SmallArithmetic::printTable(const vector<vector<string>>& table) const {
    cout << "     ";
    for (const auto& elem : alphabet) {
        cout << setw(4) << elem;
    }
    cout << endl;

    for (int i = 0; i < N; ++i) {
        cout << setw(4) << alphabet[i] << " ";
        for (int j = 0; j < N; ++j) {
            cout << setw(4) << table[i][j];
        }
        cout << endl;
    }
}

void SmallArithmetic::printAddTable() const {
    cout << "============ ТАБЛИЦА СЛОЖЕНИЯ (+) ============" << endl;
    printTable(addTable);
}

void SmallArithmetic::printMulTable() const {
    cout << "=========== ТАБЛИЦА УМНОЖЕНИЯ (*) ===========" << endl;
    printTable(mulTable);
}

void SmallArithmetic::printSubTable() const {
    cout << "=========== ТАБЛИЦА ВЫЧИТАНИЯ (-) ===========" << endl;
    printTable(subTable);
}

void SmallArithmetic::printDivTable() const {
    cout << "=========== ТАБЛИЦА ДЕЛЕНИЯ (/) ===========" << endl;
    printTable(divTable);
}

void SmallArithmetic::printAllTables() const {
    printAddTable();
    printMulTable();
    printSubTable();
    printDivTable();
}

void SmallArithmetic::printHasseDiagram() const {
    cout << "=========== ДИАГРАММА ХАССЕ (правило +1) ===========" << endl;

    cout << "\nОтношение порядка элементов Z" << N << ":\n\n  ";

    string current = additiveIdentity;
    for (int i = 0; i < N; ++i) {
        cout << current;
        if (i < N - 1) {
            cout << " => ";
        }
        current = nextSymbol(current);
    }
    cout << " => " << additiveIdentity << " (цикл)" << endl;

    cout << "\n===================================================\n" << endl;
}


