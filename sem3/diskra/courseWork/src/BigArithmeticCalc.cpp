#include "../include/BigArithmeticCalc.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <iostream>

using namespace std;

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С СИМВОЛАМИ ============

string BigArithmeticCalc::nextSymbol(const string& current) const {
    auto it = plusOneRule.find(current);
    if (it == plusOneRule.end()) {
        throw runtime_error("Invalid symbol: " + current);
    }
    return it->second;
}

string BigArithmeticCalc::prevSymbol(const string& current) const {
    // Находим предыдущий символ в цикле
    string prev = additiveIdentity;
    string curr = additiveIdentity;
    
    while (curr != current) {
        prev = curr;
        curr = nextSymbol(curr);
    }
    
    return prev;
}

int BigArithmeticCalc::getSymbolValue(const string& symbol) const {
    // Возвращает позицию символа в цикле (0..N-1)
    int value = 0;
    string current = additiveIdentity;
    
    while (current != symbol && value < N) {
        current = nextSymbol(current);
        value++;
    }
    
    return value;
}

int BigArithmeticCalc::compareSymbols(const string& a, const string& b) const {
    if (a == b) return 0;
    
    int val_a = getSymbolValue(a);
    int val_b = getSymbolValue(b);
    
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

// ============ КОНСТРУКТОР ============

BigArithmeticCalc::BigArithmeticCalc(int n, 
                    const map<string, string>& rule, 
                    const vector<string>& alph,
                    const string& addId,
                    const string& mulId)
    : N(n), plusOneRule(rule), alphabet(alph), 
      additiveIdentity(addId), multiplicativeIdentity(mulId),
      universum("[min;max]"), emptySet("∅") {
    buildNegationMap();
    buildAdditionTableWithCarry();
    buildInverseMap();
    buildAddTable();
    buildMulTable();
    buildSubTable();
    buildDivTable();
}

// ============ МАЛАЯ АРИФМЕТИКА (1 разряд) ============

string BigArithmeticCalc::addByHasse(const string& a, const string& b) const {
    if (b == additiveIdentity) return a;
    
    string counter = additiveIdentity;
    string result = a;
    
    while (counter != b) {
        result = plusOneRule.at(result);
        counter = plusOneRule.at(counter);
    }
    
    return result;
}

string BigArithmeticCalc::multiplyByHasse(const string& a, const string& b) const {
    // Специальный случай: a * a = универсум
    if (a == additiveIdentity && b == additiveIdentity) {
        return universum;
    }
    
    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }
    
    string counter = additiveIdentity;
    string result = additiveIdentity;
    
    while (counter != b) {
        result = addByHasse(result, a);
        counter = plusOneRule.at(counter);
    }
    
    return result;
}

string BigArithmeticCalc::subtractByHasse(const string& a, const string& b) const {
    // Ищем c такое, что addByHasse(b, c) == a
    for (const auto& candidate : alphabet) {
        if (addByHasse(b, candidate) == a) {
            return candidate;
        }
    }
    return "ERR";
}

string BigArithmeticCalc::divideByHasse(const string& a, const string& b) const {
    // Деление на ноль = пустое множество
    if (b == additiveIdentity) {
        return emptySet;
    }
    
    if (inverseMap.find(b) == inverseMap.end()) {
        return "ERR";
    }
    
    string bInverse = inverseMap.at(b);
    return multiplyByHasse(a, bInverse);
}

optional<string> BigArithmeticCalc::findMultiplicativeInverse(const string& x) {
    if (x == additiveIdentity) return nullopt;
    
    for (const auto& candidate : alphabet) {
        string product = multiplyByHasse(x, candidate);
        if (product == multiplicativeIdentity) {
            return candidate;
        }
    }
    return nullopt;
}

void BigArithmeticCalc::buildInverseMap() {
    for (const auto& elem : alphabet) {
        auto inv = findMultiplicativeInverse(elem);
        if (inv.has_value()) {
            inverseMap[elem] = inv.value();
        }
    }
}

void BigArithmeticCalc::buildAddTable() {
    addTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            addTable[i][j] = addByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void BigArithmeticCalc::buildMulTable() {
    mulTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mulTable[i][j] = multiplyByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void BigArithmeticCalc::buildSubTable() {
    subTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            subTable[i][j] = subtractByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void BigArithmeticCalc::buildDivTable() {
    divTable.assign(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            divTable[i][j] = divideByHasse(alphabet[i], alphabet[j]);
        }
    }
}

void BigArithmeticCalc::buildNegationMap() {
    // Строим таблицу отрицаний: x + neg(x) = additiveIdentity
    for (const auto& elem : alphabet) {
        for (const auto& candidate : alphabet) {
            if (addByHasse(elem, candidate) == additiveIdentity) {
                negationMap[elem] = candidate;
                break;
            }
        }
    }
}

void BigArithmeticCalc::buildAdditionTableWithCarry() {
    // Строим таблицу сложения с переносами: (c1, c2, carry_in) -> (sum, carry_out)
    for (const auto& c1 : alphabet) {
        for (const auto& c2 : alphabet) {
            for (const auto& carry_in : alphabet) {
                // Складываем три символа: c1 + c2 + carry_in
                string sum1 = addByHasse(c1, c2);
                string sum = addByHasse(sum1, carry_in);
                
                // Определяем перенос
                // Считаем общее значение в десятичной системе
                int total = getSymbolValue(c1) + getSymbolValue(c2) + getSymbolValue(carry_in);
                
                // Если сумма >= N, есть перенос
                string carry_out = (total >= N) ? multiplicativeIdentity : additiveIdentity;
                
                additionTableWithCarry[make_tuple(c1, c2, carry_in)] = make_pair(sum, carry_out);
            }
        }
    }
}

// ============ БОЛЬШАЯ АРИФМЕТИКА (8 разрядов) ============

bool BigArithmeticCalc::isValidNumber(const string& num) const {
    if (num.empty()) return false;
    for (char c : num) {
        if (!isValidElement(string(1, c))) return false;
    }
    return true;
}

string BigArithmeticCalc::normalize(const string& num) const {
    if (num.empty()) return additiveIdentity;
    
    // Удаляем ведущие нули (аддитивная единица)
    size_t firstNonZero = 0;
    while (firstNonZero < num.length() && 
           string(1, num[firstNonZero]) == additiveIdentity) {
        firstNonZero++;
    }
    
    if (firstNonZero == num.length()) {
        return additiveIdentity;
    }
    
    string result = num.substr(firstNonZero);
    
    // Ограничиваем до 8 разрядов
    if (result.length() > 8) {
        result = result.substr(0, 8);
    }
    
    return result;
}

string BigArithmeticCalc::addBig(const string& a, const string& b) const { 
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }
    
    string num1 = a;
    string num2 = b;

    // Выравниваем длины
    size_t max_len = max(num1.length(), num2.length());
    while (num1.length() < max_len) {
        num1 = additiveIdentity + num1;
    }
    while (num2.length() < max_len) {
        num2 = additiveIdentity + num2;
    }

    string result;
    string carry = additiveIdentity;

    // Складываем справа налево используя таблицу с переносами
    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        // Используем таблицу сложения с переносами
        auto key = make_tuple(digit1, digit2, carry);
        auto it = additionTableWithCarry.find(key);
        if (it == additionTableWithCarry.end()) {
            return "ERR: Invalid addition";
        }
        
        string sum = it->second.first;
        carry = it->second.second;
        
        result = sum + result;
    }

    // Если остался перенос, добавляем старший разряд (если не превышаем 8 разрядов)
    if (carry != additiveIdentity && result.length() < 8) {
        result = carry + result;
    }

    return normalize(result);
}

string BigArithmeticCalc::subtractBig(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }

    // Сравниваем числа
    int cmp = compareBig(a, b);
    if (cmp == 0) {
        return additiveIdentity;
    }
    
    // Определяем большее и меньшее число
    string larger = (cmp >= 0) ? a : b;
    string smaller = (cmp >= 0) ? b : a;

    // Выравниваем длины
    size_t max_len = max(larger.length(), smaller.length());
    while (larger.length() < max_len) {
        larger = additiveIdentity + larger;
    }
    while (smaller.length() < max_len) {
        smaller = additiveIdentity + smaller;
    }

    string result;
    string borrow = additiveIdentity;

    // Вычитаем справа налево
    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string current(1, larger[i]);
        
        // Вычитаем borrow из предыдущего разряда
        if (borrow != additiveIdentity) {
            // borrow всегда равен multiplicativeIdentity (1), вычитаем его
            string neg_borrow = negationMap.at(borrow);
            current = addByHasse(current, neg_borrow);
            borrow = additiveIdentity;
        }
        
        string digit2(1, smaller[i]);
        
        // Если текущая цифра меньше вычитаемой, берем заем
        if (compareSymbols(current, digit2) < 0) {
            borrow = multiplicativeIdentity;
            // Добавляем N к текущей цифре (это эквивалентно добавлению additiveIdentity после N шагов)
            string mod_value = additiveIdentity;
            for (int j = 0; j < N; j++) {
                mod_value = nextSymbol(mod_value);
            }
            current = addByHasse(current, mod_value);
        }
        
        // Вычитаем: current - digit2 = current + (-digit2)
        string neg_digit2 = negationMap.at(digit2);
        string result_digit = addByHasse(current, neg_digit2);
        
        result = result_digit + result;
    }

    return normalize(result);
}

string BigArithmeticCalc::multiplyByDigit(const string& num, const string& digit) const {
    if (digit == additiveIdentity) return additiveIdentity;
    if (digit == multiplicativeIdentity) return num;
    
    string result = additiveIdentity;
    string counter = additiveIdentity;
    
    // Умножаем через многократное сложение
    while (counter != digit) {
        result = addBig(result, num);
        counter = nextSymbol(counter);
    }
    
    return result;
}

string BigArithmeticCalc::multiplyBig(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }
    
    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }
    
    string result = additiveIdentity;
    string multiplicand = a;
    
    // Умножение столбиком: проходим по каждой цифре множителя справа налево
    for (int i = b.length() - 1; i >= 0; --i) {
        string multiplier_digit(1, b[i]);
        
        if (multiplier_digit != additiveIdentity) {
            // Умножаем multiplicand на текущую цифру множителя
            string partial_product = multiplyByDigit(multiplicand, multiplier_digit);
            
            // Сдвиг влево (добавляем нули справа)
            int shift_count = b.length() - 1 - i;
            string shifted_value = partial_product;
            for (int j = 0; j < shift_count; j++) {
                shifted_value += additiveIdentity;
            }
            
            // Ограничиваем до 8 разрядов
            if (shifted_value.length() > 8) {
                shifted_value = shifted_value.substr(shifted_value.length() - 8);
            }
            
            result = addBig(result, shifted_value);
        }
    }
    
    return normalize(result);
}



string BigArithmeticCalc::shiftLeft(const string& num, int positions) const {
    if (positions == 0 || num == additiveIdentity) return num;

    string result = num;
    for (int i = 0; i < positions; ++i) {
        result += additiveIdentity; // добавляем младший разряд справа
    }

    // Ограничиваем до 8 разрядов, оставляя старшие
    if (static_cast<int>(result.length()) > 8) {
        result = result.substr(result.length() - 8);
    }

    return normalize(result);
}


int BigArithmeticCalc::compareBig(const string& a, const string& b) const {
    string na = normalize(a);
    string nb = normalize(b);
    
    if (na.length() != nb.length()) {
        return na.length() > nb.length() ? 1 : -1;
    }
    
    for (size_t i = 0; i < na.length(); ++i) {
        int cmp = compareSymbols(string(1, na[i]), string(1, nb[i]));
        if (cmp != 0) return cmp;
    }
    
    return 0; // равны
}

pair<string, string> BigArithmeticCalc::divideBig(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return {"ERR", "ERR"};
    }
    
    // Деление на ноль
    if (b == additiveIdentity) {
        return {emptySet, emptySet};
    }
    
    // Если делимое равно нулю
    if (a == additiveIdentity) {
        return {additiveIdentity, additiveIdentity};
    }
    
    string dividend = a;
    string divisor = b;
    
    // Если делимое меньше делителя
    if (compareBig(dividend, divisor) < 0) {
        return {additiveIdentity, dividend};
    }
    
    // Деление в столбик
    string quotient_str;
    string current_remainder = additiveIdentity;
    
    // Проходим по каждой цифре делимого слева направо
    for (size_t i = 0; i < dividend.length(); i++) {
        // Добавляем следующую цифру к остатку
        current_remainder = current_remainder + string(1, dividend[i]);
        current_remainder = normalize(current_remainder);
        
        // Ищем максимальную цифру частного
        string quotient_digit = additiveIdentity;
        
        // Пробуем все возможные цифры от 1 до максимальной
        string test_digit = multiplicativeIdentity;
        while (test_digit != additiveIdentity) {
            string test_product = multiplyByDigit(divisor, test_digit);
            
            if (compareBig(test_product, current_remainder) > 0) {
                break;  // Превысили остаток
            }
            
            quotient_digit = test_digit;
            test_digit = nextSymbol(test_digit);
        }
        
        quotient_str += quotient_digit;
        
        // Вычитаем произведение из остатка
        if (quotient_digit != additiveIdentity) {
            string product = multiplyByDigit(divisor, quotient_digit);
            current_remainder = subtractBig(current_remainder, product);
        }
    }
    
    string quotient = normalize(quotient_str);
    string remainder = normalize(current_remainder);
    
    return {quotient, remainder};
}


string BigArithmeticCalc::shiftRight(const string& num, int positions) const {
    if (positions == 0 || num.empty()) return num;
    if (positions >= static_cast<int>(num.length())) return additiveIdentity;
    
    return num.substr(0, num.length() - positions);
}

// ============ ПУБЛИЧНЫЕ ОПЕРАЦИИ ============

string BigArithmeticCalc::addSmall(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR";
    return addByHasse(a, b);
}

string BigArithmeticCalc::multiplySmall(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR";
    return multiplyByHasse(a, b);
}

string BigArithmeticCalc::subtractSmall(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR";
    return subtractByHasse(a, b);
}

string BigArithmeticCalc::divideSmall(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR";
    return divideByHasse(a, b);
}

string BigArithmeticCalc::add(const string& a, const string& b) const {
    return addBig(a, b);
}

string BigArithmeticCalc::multiply(const string& a, const string& b) const {
    return multiplyBig(a, b);
}

string BigArithmeticCalc::subtract(const string& a, const string& b) const {
    return subtractBig(a, b);
}

string BigArithmeticCalc::divide(const string& a, const string& b) const {
    auto [q, r] = divideBig(a, b);
    return "Q: " + q + " | R: " + r;
}

// ============ УТИЛИТЫ ============

bool BigArithmeticCalc::isValidElement(const string& elem) const {
    for (const auto& x : alphabet) {
        if (x == elem) return true;
    }
    return false;
}

bool BigArithmeticCalc::isInvertible(const string& elem) const {
    return inverseMap.find(elem) != inverseMap.end();
}

optional<string> BigArithmeticCalc::getInverse(const string& elem) const {
    auto it = inverseMap.find(elem);
    if (it != inverseMap.end()) {
        return it->second;
    }
    return nullopt;
}

// ============ ГЕТТЕРЫ ============

const map<string, string>& BigArithmeticCalc::getInverseMap() const {
    return inverseMap;
}

const vector<vector<string>>& BigArithmeticCalc::getAddTable() const {
    return addTable;
}

const vector<vector<string>>& BigArithmeticCalc::getMulTable() const {
    return mulTable;
}

const vector<vector<string>>& BigArithmeticCalc::getSubTable() const {
    return subTable;
}

const vector<vector<string>>& BigArithmeticCalc::getDivTable() const {
    return divTable;
}

const vector<string>& BigArithmeticCalc::getAlphabet() const {
    return alphabet;
}

// ============ ВЫВОД ИНФОРМАЦИИ ============

void BigArithmeticCalc::printTable(const vector<vector<string>>& table) const {
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

void BigArithmeticCalc::printAddTable() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ТАБЛИЦА СЛОЖЕНИЯ (+)              ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    printTable(addTable);
}

void BigArithmeticCalc::printMulTable() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ТАБЛИЦА УМНОЖЕНИЯ (*)             ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    printTable(mulTable);
}

void BigArithmeticCalc::printSubTable() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ТАБЛИЦА ВЫЧИТАНИЯ (-)             ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    printTable(subTable);
}

void BigArithmeticCalc::printDivTable() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ТАБЛИЦА ДЕЛЕНИЯ (/)               ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    printTable(divTable);
}

void BigArithmeticCalc::printAllTables() const {
    printAddTable();
    printMulTable();
    printSubTable();
    printDivTable();
}

void BigArithmeticCalc::printHasseDiagram() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║          ДИАГРАММА ХАССЕ (правило +1)                  ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\nЦиклический порядок элементов Z" << N << ":\n\n  ";
    
    string current = additiveIdentity;
    for (int i = 0; i < N; ++i) {
        cout << current;
        if (i < N - 1) {
            cout << " → ";
        }
        current = plusOneRule.at(current);
    }
    cout << " → " << additiveIdentity << " (цикл)" << endl;
    
    cout << "\n════════════════════════════════════════════════════════\n" << endl;
}

void BigArithmeticCalc::printInvertibleElements() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║              ОБРАТИМЫЕ ЭЛЕМЕНТЫ                        ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝\n" << endl;
    
    if (inverseMap.empty()) {
        cout << "Нет обратимых элементов\n" << endl;
        return;
    }
    
    cout << "Всего обратимых: " << inverseMap.size() << "\n" << endl;
    
    for (const auto& [elem, inv] : inverseMap) {
        cout << "  " << elem << "⁻¹ = " << inv << endl;
    }
    
    cout << "\n════════════════════════════════════════════════════════\n" << endl;
}

void BigArithmeticCalc::printInfo() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║         ИНФОРМАЦИЯ О КОНЕЧНОЙ АРИФМЕТИКЕ               ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\nРазмерность: Z" << N << endl;
    cout << "Аддитивная единица (0): '" << additiveIdentity << "'" << endl;
    cout << "Мультипликативная единица (1): '" << multiplicativeIdentity << "'" << endl;
    cout << "\nМаксимальное количество разрядов: 8" << endl;
    
    cout << "\nСпециальные правила:" << endl;
    cout << "  • " << additiveIdentity << " * " << additiveIdentity 
         << " = " << universum << " (универсум)" << endl;
    cout << "  • число / " << additiveIdentity << " = " << emptySet 
         << " (пустое множество)" << endl;
    
    cout << "\n════════════════════════════════════════════════════════\n" << endl;
}

void BigArithmeticCalc::printHelp() const {
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║                      ПОМОЩЬ                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\nДоступные команды:\n" << endl;
    cout << "  МАЛАЯ АРИФМЕТИКА (1 разряд):" << endl;
    cout << "    <элемент> + <элемент>   - сложение" << endl;
    cout << "    <элемент> - <элемент>   - вычитание" << endl;
    cout << "    <элемент> * <элемент>   - умножение" << endl;
    cout << "    <элемент> / <элемент>   - деление" << endl;
    
    cout << "\n  БОЛЬШАЯ АРИФМЕТИКА (до 8 разрядов):" << endl;
    cout << "    <число> ++ <число>       - сложение больших чисел" << endl;
    cout << "    <число> -- <число>       - вычитание больших чисел" << endl;
    cout << "    <число> ** <число>       - умножение больших чисел" << endl;
    cout << "    <число> // <число>       - деление больших чисел" << endl;
    
    cout << "\n  ИНФОРМАЦИЯ:" << endl;
    cout << "    info                    - информация об арифметике" << endl;
    cout << "    hasse                   - диаграмма Хассе" << endl;
    cout << "    inv_list                - список обратимых элементов" << endl;
    cout << "    inv <элемент>           - обратный элемент" << endl;
    cout << "    tables                  - все таблицы операций" << endl;
    cout << "    add_table               - таблица сложения" << endl;
    cout << "    mul_table               - таблица умножения" << endl;
    cout << "    sub_table               - таблица вычитания" << endl;
    cout << "    div_table               - таблица деления" << endl;
    cout << "    help                    - эта справка" << endl;
    cout << "    exit / quit             - выход" << endl;
    
    cout << "\nПримеры:" << endl;
    cout << "  Малая: b + c, h * d" << endl;
    cout << "  Большая: abc ++ def, gh -- cd, abcd ** ef, hgfedcba // abc" << endl;
    
    cout << "\n════════════════════════════════════════════════════════\n" << endl;
}
