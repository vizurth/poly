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

int BigArithmeticCalc::compareSymbols(const string& a, const string& b) const {
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

// ============ МАЛАЯ АРИФМЕТИКА ============

string BigArithmeticCalc::addByHasse(const string& a, const string& b) const {
    if (b == additiveIdentity) return a;
    
    string counter = additiveIdentity;
    string result = a;
    
    while (counter != b) {
        result = nextSymbol(result);
        counter = nextSymbol(counter);
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
        counter = nextSymbol(counter);
    }
    
    return result;
}

string BigArithmeticCalc::subtractByHasse(const string& a, const string& b) const {
    // ищем с такое, что addByHasse(b, c) == a
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

// Построение вспомогательных таблиц
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

// Основная таблица сложения с переносами
void BigArithmeticCalc::buildAdditionTableWithCarry() {

    for (const auto& c1 : alphabet) {
        for (const auto& c2 : alphabet) {
            for (const auto& carry_in : alphabet) {
                string sum1 = addByHasse(c1, c2);
                string final_sum = addByHasse(sum1, carry_in);
                
                // Определяем, был ли перенос
                // Перенос есть, если:
                // 1. c2 != additiveIdentity и sum1 < c1 (первое сложение перешло через ноль)
                // 2. carry_in != additiveIdentity и final_sum < sum1 (второе сложение перешло через ноль)
                
                bool carry1 = false;
                bool carry2 = false;
                
                // Проверяем первый перенос: c1 + c2
                if (c2 != additiveIdentity) {
                    // Идем от c1 на c2 шагов и проверяем, прошли ли через additiveIdentity
                    string temp = c1;
                    string counter = additiveIdentity;
                    while (counter != c2) {
                        string old_temp = temp;
                        temp = nextSymbol(temp);
                        counter = nextSymbol(counter);
                        
                        // Если прошли через additiveIdentity
                        if (old_temp != additiveIdentity && temp == additiveIdentity) {
                            carry1 = true;
                        }
                    }
                }
                
                // Проверяем второй перенос: sum1 + carry_in
                if (carry_in != additiveIdentity) {
                    string temp = sum1;
                    string counter = additiveIdentity;
                    while (counter != carry_in) {
                        string old_temp = temp;
                        temp = nextSymbol(temp);
                        counter = nextSymbol(counter);
                        
                        if (old_temp != additiveIdentity && temp == additiveIdentity) {
                            carry2 = true;
                        }
                    }
                }
                
                // Если был хотя бы один перенос, выставляем carry_out = multiplicativeIdentity
                string carry_out = (carry1 || carry2) ? multiplicativeIdentity : additiveIdentity;
                
                additionTableWithCarry[make_tuple(c1, c2, carry_in)] = make_pair(final_sum, carry_out);
            }
        }
    }
}
// Debug
void BigArithmeticCalc::printAdditionTableWithCarry() const {
    for (const auto& c1 : alphabet) {
        for (const auto& c2 : alphabet) {
            for (const auto& carry_in : alphabet) {

                auto key = make_tuple(c1, c2, carry_in);
                auto it = additionTableWithCarry.find(key);

                if (it == additionTableWithCarry.end()) {
                    cout << "No entry for (" << c1 << ", " << c2 << ", " << carry_in << ")\n";
                    continue;
                }

                const auto& [sum, carry_out] = it->second;

                cout << "c1=" << c1
                     << "  c2=" << c2
                     << "  carry_in=" << carry_in
                     << "  => sum=" << sum 
                     << "  carry_out=" << carry_out
                     << "\n";
            }
        }
        cout << "-----------------------------\n";
    }
}


// ============ БОЛЬШАЯ АРИФМЕТИКА (8 разрядов) ============
// проверка не вылезает ли строка за пределы алфавита
bool BigArithmeticCalc::isValidNumber(const string& num) const {
    if (num.empty()) return false;
    for (char c : num) {
        if (!isValidElement(string(1, c))) return false;
    }
    return true;
}
// Удаление ведущих нулей и ограничение до 8 разрядов
string BigArithmeticCalc::normalize(const string& num) const {
    if (num.empty()) return additiveIdentity;
    
    // Удаляем ведущие нули (аддитивная единица)
    size_t firstNonZero = 0;
    while (firstNonZero < num.length() && string(1, num[firstNonZero]) == additiveIdentity) {
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

    // выравниваем длины
    size_t max_len = max(num1.length(), num2.length());
    while (num1.length() < max_len) {
        num1 = additiveIdentity + num1;
    }
    while (num2.length() < max_len) {
        num2 = additiveIdentity + num2;
    }

    string result;
    string carry = additiveIdentity;

    // складываем справа налево используя таблицу с переносами
    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        // используем таблицу сложения с переносами
        auto key = make_tuple(digit1, digit2, carry);
        auto it = additionTableWithCarry.find(key);
        if (it == additionTableWithCarry.end()) {
            return "ERR: Invalid addition";
        }
        
        string sum = it->second.first;
        carry = it->second.second;
        
        result = sum + result;
    }

    // если остался перенос добавим к старшему разряд
    if (carry != additiveIdentity && result.length() < 8) {
        result = carry + result;
    }

    return normalize(result);
}

string BigArithmeticCalc::subtractBig(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }

    // сравниваем числа
    int cmp = compareBig(a, b);
    if (cmp == 0) {
        return additiveIdentity;
    }
    
    // определяем большее и меньшее число и меняем их местами
    string larger = (cmp >= 0) ? a : b;
    string smaller = (cmp >= 0) ? b : a;

    // выравниваем длины
    size_t max_len = max(larger.length(), smaller.length());
    while (larger.length() < max_len) {
        larger = additiveIdentity + larger;
    }
    while (smaller.length() < max_len) {
        smaller = additiveIdentity + smaller;
    }

    string result;
    string borrow = additiveIdentity;

    // вычитаем справа налево
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
    
    // умножаем через многократное сложение
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
    
    // умножение столбиком: проходим по каждой цифре множителя справа налево
    for (int i = b.length() - 1; i >= 0; --i) {
        string multiplier_digit(1, b[i]);
        
        if (multiplier_digit != additiveIdentity) {
            // умножаем multiplicand на текущую цифру множителя
            string partial_product = multiplyByDigit(multiplicand, multiplier_digit);
            
            // сдвиг влево (добавляем нули справа)
            int shift_count = b.length() - 1 - i;
            string shifted_value = partial_product;
            for (int j = 0; j < shift_count; j++) {
                shifted_value += additiveIdentity;
            }
            // ограничиваем до 8 разрядов
            if (shifted_value.length() > 8) {
                shifted_value = shifted_value.substr(shifted_value.length() - 8);
            }
            
            result = addBig(result, shifted_value);
        }
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

// ============ ПУБЛИЧНЫЕ ОПЕРАЦИИ ============
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

// ============ ГЕТТЕРЫ ============
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
            cout << " => ";
        }
        current = nextSymbol(current);
    }
    cout << " → " << additiveIdentity << " (цикл)" << endl;
    
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
	printAdditionTableWithCarry();
    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║                      ПОМОЩЬ                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\nДоступные команды:\n" << endl;
    cout << "\n  БОЛЬШАЯ АРИФМЕТИКА (до 8 разрядов):" << endl;
    cout << "    <число> + <число>       - сложение больших чисел" << endl;
    cout << "    <число> - <число>       - вычитание больших чисел" << endl;
    cout << "    <число> * <число>       - умножение больших чисел" << endl;
    cout << "    <число> / <число>       - деление больших чисел" << endl;
    
    cout << "\n  ИНФОРМАЦИЯ:" << endl;
    cout << "    info                    - информация об арифметике" << endl;
    cout << "    hasse                   - диаграмма Хассе" << endl;
    cout << "    tables                  - все таблицы операций" << endl;
    cout << "    add_table               - таблица сложения" << endl;
    cout << "    mul_table               - таблица умножения" << endl;
    cout << "    sub_table               - таблица вычитания" << endl;
    cout << "    div_table               - таблица деления" << endl;
    cout << "    help                    - эта справка" << endl;
    cout << "    exit / quit             - выход" << endl;
    
    cout << "\nПримеры:" << endl;
    cout << "  abc + def, gh - cd, abcd * ef, hgfedcba / abc" << endl;
    
    cout << "\n════════════════════════════════════════════════════════\n" << endl;
}
