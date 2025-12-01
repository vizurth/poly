#include "../include/BigArithmeticCalc.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <iostream>

using namespace std;

// ============ вспомогательные функции для работы с символами ============

// следующий символ по правилу +1
string BigArithmeticCalc::nextSymbol(const string& current) const {
    auto it = plusOneRule.find(current);
    if (it == plusOneRule.end()) {
        throw runtime_error("invalid symbol: " + current);
    }
    return it->second;
}

// функция для сравнения символов
/*
	если a == b или символы не найдены 0
	если a > b => 1
	ecли a < b => -1
	ходим по бесконечному циклу через правило +1
*/
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

// ============ малая арифметика ============

// базовые операции через диаграмму хассе

/*
	LOOK: addByHasse(a, b)
	будем поддерживать 
	counter = additiveIdentity(ноль)
	result = a
	(
	если смотреть на диаграмму хассе то мы будем прибавлять к result +1 и ходить через nextSymbol
	до момента пока counter != b
	)
*/
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

/*
	LOOK: multiplyByHasse(a, b)
	Проверяем граничные случаи

	так как мы умножаем мы должны несколько раз прибавлять a к результату
	до того момента пока counter != b(то сколько раз надо умножить)

	result = addByHasse(result, a)
*/
string BigArithmeticCalc::multiplyByHasse(const string& a, const string& b) const {
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

/*
	LOOK: subtractByHasse(a, b)
	ищем c такое, что addByHasse(b, c) == a
	перебираем все элементы алфавита
*/
string BigArithmeticCalc::subtractByHasse(const string& a, const string& b) const {
    for (const auto& candidate : alphabet) {
        if (addByHasse(b, candidate) == a) {
            return candidate;
        }
    }
    return "ERR: with subtract";
}

/*
	LOOK: divideByHasse(a, b)
	деление на ноль = пустое множество
	деление будет равняться a * b^-1(инвертированное)
*/
string BigArithmeticCalc::divideByHasse(const string& a, const string& b) const {
    if (b == additiveIdentity) {
        return emptySet;
    }
    
    if (inverseMap.find(b) == inverseMap.end()) {
        return "ERR: cant find b^-1";
    }
    
    string bInverse = inverseMap.at(b);
    return multiplyByHasse(a, bInverse);
}

/*
	LOOK: findMultiplicativeInverse(x)
	ищем такое что candidate что x * candidate = multiplicativeIdentity
*/
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

// построение вспомогательных таблиц
void BigArithmeticCalc::buildInverseMap() {
    for (const auto& elem : alphabet) {
        auto inv = findMultiplicativeInverse(elem);
        if (inv.has_value()) {
            inverseMap[elem] = inv.value();
        }
    }
}


/*
	LOOK: блок кода с построением таблиц
*/
void BigArithmeticCalc::buildAddTable() {
    addTable.assign(N, vector<string>(N));
    // используем буквенные индексы
    string i = additiveIdentity;
    for (int idx_i = 0; idx_i < N; ++idx_i) {
        string j = additiveIdentity;
        for (int idx_j = 0; idx_j < N; ++idx_j) {
            addTable[idx_i][idx_j] = addByHasse(alphabet[idx_i], alphabet[idx_j]);
            j = nextSymbol(j);
        }
        i = nextSymbol(i);
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
    // строим таблицу отрицаний: x + neg(x) = additiveIdentity
    for (const auto& elem : alphabet) {
        for (const auto& candidate : alphabet) {
            if (addByHasse(elem, candidate) == additiveIdentity) {
                negationMap[elem] = candidate;
                break;
            }
        }
    }
}

/*
	LOOK: buildAdditionTableWithCarry()
	структура таблицы (c1, c2, carry_in) => (sum, carry)
	функция построения таблицы сложения с учетом переносов из младших разрядов

	запишем все значения для этого будем перебирать
	с1 - первый слагаемое
	с2 - второе слагаемое
	carry_in - перенос из младшего разряда

	сумма = addByHasse(addByHasse(c1, c2), carry_in)
	так же будем проверять прошли ли мы полностью диаграмму функцией 
	LOOK: checkCarry(start, steps)
*/
void BigArithmeticCalc::buildAdditionTableWithCarry() {
    for (const auto& c1 : alphabet) {
        for (const auto& c2 : alphabet) {
			//FIXME: возможно тут надо итерироваться по всем символам алфавита
            for (const auto& carry_in : {additiveIdentity, multiplicativeIdentity}) {
                string sum1 = addByHasse(c1, c2); // сумма без переноса
                string final_sum = addByHasse(sum1, carry_in); // финальная сумма с учетом переноса
                
                bool carry1 = checkCarry(c1, c2); // проверяем перенос с1 + с2
				bool carry2 = checkCarry(sum1, carry_in); // проверяем перенос sum1 + carry_in
                
                string carry_out = (carry1 || carry2) ? multiplicativeIdentity : additiveIdentity; // выставляем перенос при 
                
                additionTableWithCarry[make_tuple(c1, c2, carry_in)] = make_pair(final_sum, carry_out);
            }
        }
    }
}

/*
	LOOK: checkCarry(start, steps)
	проверяет возникновение переноса при сложении start + steps
	если перенос возник true иначе false
*/
bool BigArithmeticCalc::checkCarry(const string& start, const string& steps) const {
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

// ============ работа со знаками минус ============

bool BigArithmeticCalc::isNegative(const string& num) const {
    return !num.empty() && num[0] == '-';
}

string BigArithmeticCalc::removeSign(const string& num) const {
    if (isNegative(num)) {
        return num.substr(1);
    }
    return num;
}

string BigArithmeticCalc::addSign(const string& num, bool negative) const {
    if (num == additiveIdentity) return num;
    return negative ? "-" + num : num;
}

// ============ большая арифметика  ============

// проверка не вылезает ли строка за пределы алфавита
bool BigArithmeticCalc::isValidNumber(const string& num) const {
    if (num.empty()) return false;
    
    string unsigned_num = removeSign(num);
    
    for (char c : unsigned_num) {
        if (!isValidElement(string(1, c))) return false;
    }
    return true;
}

bool BigArithmeticCalc::isValidElement(const string& elem) const {
    for (const auto& x : alphabet) {
        if (x == elem) return true;
    }
    return false;
}

// проверка на переполнение по количеству разрядов
bool BigArithmeticCalc::isOverflow(const string& num) const {
    string unsigned_num = removeSign(num);
    string normalized = normalize(unsigned_num);
    return normalized.length() > MAX_DIGITS;
}

// удаление ведущих нулей

/*
	LOOK: normalize(num) 
	функция для удаление ведущих нулей
	так как мы можем при вычислении добавить ведущие нули для упрощения вычислений
	бедем счетчиком считать количество нулей спереди числа а далее просто обрежем их
*/
string BigArithmeticCalc::normalize(const string& num) const {
    if (num.empty()) return additiveIdentity;
    
    bool negative = isNegative(num);
    string unsigned_num = removeSign(num);
    
    // удаляем ведущие нули
    size_t firstNonZero = 0;
    while (firstNonZero < unsigned_num.length() && 
           string(1, unsigned_num[firstNonZero]) == additiveIdentity) {
        firstNonZero++;
    }
    
    if (firstNonZero == unsigned_num.length()) {
        return additiveIdentity;
    }
    
    string result = unsigned_num.substr(firstNonZero);
    
    return addSign(result, negative);
}

/*
	LOOK: addBigUnsigned(a, b)
	функция для беззнакового суммирования чисел столбиком
	для этого просто будем поддерживать переменные:
	result, carry
	проходим по разрядам справа налево и для каждого разряда
	берем digit1 и digit2 cуммируем их по таблице с переносами
	и записываем carry != additiveIdentity
	в самом конце приписываем carry к результату если он есть
	
	проверяем на переполнение
*/
string BigArithmeticCalc::addBigUnsigned(const string& a, const string& b) const { 
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: invalid number";
    }
    
    string num1 = a;
    string num2 = b;

    // добавляем нули
    size_t max_len = max(num1.length(), num2.length());
    while (num1.length() < max_len) {
        num1 = additiveIdentity + num1;
    }
    while (num2.length() < max_len) {
        num2 = additiveIdentity + num2;
    }

    string result;
    string carry = additiveIdentity;

    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        auto key = make_tuple(digit1, digit2, carry);
        auto it = additionTableWithCarry.find(key);
        if (it == additionTableWithCarry.end()) {
            return "ERR: invalid addition";
        }
        
        string sum = it->second.first;
        carry = it->second.second;
        
        result = sum + result;
    }

    if (carry != additiveIdentity) {
        result = carry + result;
    }

    if (isOverflow(result)) {
        return "ERR: overflow";
    }

    return normalize(result);
}

/*
	LOOK: subtractBigUnsigned(a, b)
	тут пофакту считаем модуль вычитание столбиком
	поддерживаем result и borrow 
	также смотрим на заем из младшего разряда
	и используем substractByHasse(a, b)
*/
string BigArithmeticCalc::subtractBigUnsigned(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: invalid number";
    }

    // сравниваем числа
    int cmp = compareBigUnsigned(a, b);
    if (cmp == 0) {
        return additiveIdentity;
    }
    
    // определяем большее и меньшее число
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
        
        // вычитаем заем из предыдущего разряда
        if (borrow != additiveIdentity) {
            // string neg_borrow = negationMap.at(borrow);
            // current = addByHasse(current, neg_borrow);
			// FIXME: используем вычитание по диаграмме хассе
			current = subtractByHasse(current, borrow);
            borrow = additiveIdentity;
        }
        
        string digit2(1, smaller[i]);
        
        // если текущая цифра меньше вычитаемой, берем заем
        if (compareSymbols(current, digit2) < 0) {
            borrow = multiplicativeIdentity;
            // добавляем N к текущей цифре
            string counter = additiveIdentity;
            for (int j = 0; j < N; j++) {
                current = nextSymbol(current);
            }
        }
        
        // вычитаем: current - digit2 = current + (-digit2)
        // string neg_digit2 = negationMap.at(digit2);
        // string result_digit = addByHasse(current, neg_digit2);
		// FIXME: используем вычитание по диаграмме хассе
		string result_digit = subtractByHasse(current, digit2);
        
        result = result_digit + result;
    }

    return normalize(result);
}

/*
	LOOK: multiplyByDigit(num, digit)
	функция умножения числа на цифру
	просто будем использовать counter чтолбы считать сколько раз уже умножили 
	и result = addBigUnsigned(result, num)
*/
string BigArithmeticCalc::multiplyByDigit(const string& num, const string& digit) const {
    if (digit == additiveIdentity) return additiveIdentity;
    if (digit == multiplicativeIdentity) return num;
    
    string result = additiveIdentity;
    string counter = additiveIdentity;
    
    // умножаем через многократное сложение с буквенным счетчиком
    while (counter != digit) {
        result = addBigUnsigned(result, num);
        if (result.substr(0, 4) == "ERR:") return result;
        counter = nextSymbol(counter);
    }
    
    return result;
}

/*
	LOOK: multiplyBigUnsigned(a, b)
	функция умножения чисел столбиком
	проходим по каждой цифре множителя справа налево
	для каждой цифры получаем частичное произведение
	и сдвигаем его влево на соответствующее количество разрядов
*/
string BigArithmeticCalc::multiplyBigUnsigned(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: invalid number";
    }
    
    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }
    
    string result = additiveIdentity;
    
    // умножение столбиком: проходим по каждой цифре множителя справа налево
    for (int i = b.length() - 1; i >= 0; --i) {
        string multiplier_digit(1, b[i]);
        
        if (multiplier_digit != additiveIdentity) {
            string partial_product = multiplyByDigit(a, multiplier_digit);
            if (partial_product.substr(0, 4) == "ERR:") return partial_product;
            
            // сдвиг влево (добавляем нули справа)
            int shift_count = b.length() - 1 - i;
            for (int j = 0; j < shift_count; j++) {
                partial_product += additiveIdentity;
            }
            
            result = addBigUnsigned(result, partial_product);
            if (result.substr(0, 4) == "ERR:") return result;
        }
    }
    
    return normalize(result);
}

/*
	LOOK: compareBigUnsigned(a, b)
	функция для сравнения беззнаковых больших чисел
	сравниваем по длине а далее поразрядно слева направо c помощью compareSymbols
*/
int BigArithmeticCalc::compareBigUnsigned(const string& a, const string& b) const {
    string na = normalize(a);
    string nb = normalize(b);
    
    if (na.length() != nb.length()) {
        return na.length() > nb.length() ? 1 : -1;
    }
    
    for (size_t i = 0; i < na.length(); ++i) {
        int cmp = compareSymbols(string(1, na[i]), string(1, nb[i]));
        if (cmp != 0) return cmp;
    }
    
    return 0;
}

/*
	LOOK: divideBigUnsigned(a, b)
	функция для деления в столбик
*/
pair<string, string> BigArithmeticCalc::divideBigUnsigned(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return {"ERR", "ERR"};
    }
    
    // деление на ноль
    if (b == additiveIdentity) {
        // 0/0 = [min;max]
        if (a == additiveIdentity) {
            return {universum, universum};
        }
        return {emptySet, emptySet};
    }
    
    // если делимое равно нулю
    if (a == additiveIdentity) {
        return {additiveIdentity, additiveIdentity};
    }
    
    string dividend = a;
    string divisor = b;
    
    // если делимое меньше делителя
    if (compareBigUnsigned(dividend, divisor) < 0) {
        return {additiveIdentity, dividend};
    }
    
    // деление в столбик
    string q_str;
    string current_remainder = additiveIdentity;
    
    // проходим по каждой цифре делимого слева направо
    for (size_t i = 0; i < dividend.length(); i++) {
        // добавляем следующую цифру к остатку
        current_remainder = current_remainder + string(1, dividend[i]);
        current_remainder = normalize(current_remainder);
        
        // ищем максимальную цифру частного
        string q_digit = additiveIdentity;
        
        // пробуем все возможные цифры от 1 до максимальной
        string test_digit = multiplicativeIdentity;
        while (test_digit != additiveIdentity) {
            string test_product = multiplyByDigit(divisor, test_digit);
            
            if (compareBigUnsigned(test_product, current_remainder) > 0) {
                break;
            }
            
            q_digit = test_digit;
            test_digit = nextSymbol(test_digit);
        }
        
        q_str += q_digit;
        
        // вычитаем произведение из остатка
        if (q_digit != additiveIdentity) {
            string product = multiplyByDigit(divisor, q_digit);
            current_remainder = subtractBigUnsigned(current_remainder, product);
        }
    }
    
    string q = normalize(q_str);
    string remainder = normalize(current_remainder);
    
    return {q, remainder};
}

// ============ операции с учетом знаков ============

/*
	LOOK: add(a, b)
	функция для сложения больших чисел со знаком
	рассматриваем 4 случая в зависимости от знаков a и b
*/
string BigArithmeticCalc::add(const string& a, const string& b) const {
    bool neg_a = isNegative(a);
    bool neg_b = isNegative(b);
    
    string unsigned_a = removeSign(a);
    string unsigned_b = removeSign(b);
    
    // a + b (оба положительные)
    if (!neg_a && !neg_b) {
        return addBigUnsigned(unsigned_a, unsigned_b);
    }
    
    // -a + (-b) = -(a + b)
    if (neg_a && neg_b) {
        string sum = addBigUnsigned(unsigned_a, unsigned_b);
        if (sum.substr(0, 4) == "ERR:") return sum;
        return addSign(sum, true);
    }
    
    // a + (-b) = a - b
    if (!neg_a && neg_b) {
        int cmp = compareBigUnsigned(unsigned_a, unsigned_b);
        if (cmp >= 0) {
            return subtractBigUnsigned(unsigned_a, unsigned_b);
        } else {
            string diff = subtractBigUnsigned(unsigned_b, unsigned_a);
			cout << "Debug:" << diff << endl;
            return addSign(diff, true);
        }
    }
    
    // -a + b = b - a
    if (neg_a && !neg_b) {
        int cmp = compareBigUnsigned(unsigned_b, unsigned_a);
        if (cmp >= 0) {
            return subtractBigUnsigned(unsigned_b, unsigned_a);
        } else {
            string diff = subtractBigUnsigned(unsigned_a, unsigned_b);
            return addSign(diff, true);
        }
    }
    
    return "ERR: unknown case";
}

/*
	LOOK: subtract(a, b)
	функция для вычитания больших чисел со знаком
	используем правило: a - b = a + (-b)
*/
string BigArithmeticCalc::subtract(const string& a, const string& b) const {
    // a - b = a + (-b)
    bool neg_b = isNegative(b);
    string unsigned_b = removeSign(b);
    string negated_b = addSign(unsigned_b, !neg_b);
    
    return add(a, negated_b);
}
/*
	LOOK: multiply(a, b)
	функция для умножения больших чисел со знаком
	результат отрицательный, если знаки разные
*/
string BigArithmeticCalc::multiply(const string& a, const string& b) const {
    bool neg_a = isNegative(a);
    bool neg_b = isNegative(b);
    
    string unsigned_a = removeSign(a);
    string unsigned_b = removeSign(b);
    
    string product = multiplyBigUnsigned(unsigned_a, unsigned_b);
    if (product.substr(0, 4) == "ERR:") return product;
    
    // результат отрицательный, если знаки разные
    bool result_negative = (neg_a != neg_b);
    
    return addSign(product, result_negative);
}

/*
	LOOK: divide(a, b)
	функция для деления больших чисел со знаком
	обрабатываем специальные случаи
*/
string BigArithmeticCalc::divide(const string& a, const string& b) const {
    bool neg_a = isNegative(a);
    bool neg_b = isNegative(b);
    
    string unsigned_a = removeSign(a);
    string unsigned_b = removeSign(b);
    
    auto [q, r] = divideBigUnsigned(unsigned_a, unsigned_b);
    
    // обработка специальных случаев
    if (q == emptySet || q == universum) {
        return "Q: " + q + " | R: " + r;
    }
    
    if (q.substr(0, 3) == "ERR") {
        return q;
    }
    
    // если делимое отрицательное, а делитель положительный
    // то -a / b = -(a/(b + 1)), остаток = b - r
    if (neg_a && !neg_b && r != additiveIdentity) {
        // добавляем единицу к частному
        q = addBigUnsigned(q, multiplicativeIdentity);
        // вычисляем новый остаток: b - r
        r = subtractBigUnsigned(unsigned_b, r);
        q = addSign(q, true);
    }
    // если делимое положительное, а делитель отрицательный
    // то a / (-b) = -(a/b), остаток остается r
    else if (!neg_a && neg_b) {
        q = addSign(q, true);
    }
    // если оба отрицательные: -a / (-b) = a/b
    else if (neg_a && neg_b) {}
    
    return "Q: " + q + " | R: " + r;
}

// ============ геттеры ============

const vector<string>& BigArithmeticCalc::getAlphabet() const {
    return alphabet;
}

string BigArithmeticCalc::getMinNumber() const {
    string result = "-";
    for (int i = 0; i < MAX_DIGITS; i++) {
        result += alphabet.back();
    }
    return result;
}

string BigArithmeticCalc::getMaxNumber() const {
    string result;
    for (int i = 0; i < MAX_DIGITS; i++) {
        result += alphabet.back();
    }
    return result;
}

// ============ вывод менюшек ============

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
    cout << "============ ТАБЛИЦА СЛОЖЕНИЯ (+) ============" << endl;
    printTable(addTable);
}

void BigArithmeticCalc::printMulTable() const {
    cout << "=========== ТАБЛИЦА УМНОЖЕНИЯ (*) ===========" << endl;
    printTable(mulTable);
}

void BigArithmeticCalc::printSubTable() const {
    cout << "=========== ТАБЛИЦА ВЫЧИТАНИЯ (-) ===========" << endl;
    printTable(subTable);
}

void BigArithmeticCalc::printDivTable() const {
    cout << "=========== ТАБЛИЦА ДЕЛЕНИЯ (/) ===========" << endl;
    printTable(divTable);
}

void BigArithmeticCalc::printAllTables() const {
    printAddTable();
    printMulTable();
    printSubTable();
    printDivTable();
}

void BigArithmeticCalc::printHasseDiagram() const {
    cout << "=========== ДИАГРАММА ХАССЕ (правило +1) ===========" << endl;
    
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
    
    cout << "\n===================================================\n" << endl;
}

void BigArithmeticCalc::printInfo() const {
    cout << "=========== ИНФОРМАЦИЯ О КОНЕЧНОЙ АРИФМЕТИКЕ ===========" << endl;
    
    cout << "\nразмерность: Z" << N << endl;
    cout << "аддитивная единица (0): '" << additiveIdentity << "'" << endl;
    cout << "мультипликативная единица (1): '" << multiplicativeIdentity << "'" << endl;
    cout << "\nмаксимальное количество разрядов: " << MAX_DIGITS << endl;
    cout << "диапазон чисел: [" << getMinNumber() << "; " << getMaxNumber() << "]" << endl;
    
    cout << "\nспециальные правила:" << endl;
    cout << "  • " << additiveIdentity << " * " << additiveIdentity 
         << " = " << universum << " (универсум)" << endl;
    cout << "  • число / " << additiveIdentity << " = " << emptySet 
         << " (пустое множество)" << endl;
    cout << "  • " << additiveIdentity << " / " << additiveIdentity 
         << " = " << universum << " (диапазон всех чисел)" << endl;
    cout << "  • переполнение (> " << MAX_DIGITS << " разрядов) = ERR: overflow" << endl;
}

void BigArithmeticCalc::printHelp() const {
    cout << "=========== ПОМОЩЬ ===========" << endl;
    
    cout << "\nдоступные команды:\n" << endl;
    cout << "\n  БОЛЬШАЯ АРИФМЕТИКА (до " << MAX_DIGITS << " разрядов, с отрицательными):" << endl;
    cout << "    <число> + <число>       - сложение больших чисел" << endl;
    cout << "    <число> - <число>       - вычитание больших чисел" << endl;
    cout << "    <число> * <число>       - умножение больших чисел" << endl;
    cout << "    <число> / <число>       - деление больших чисел" << endl;
    cout << "\n    примечание: отрицательные числа пишутся с '-' (например: -abc)" << endl;
    
    cout << "\n  ИНФОРМАЦИЯ:" << endl;
    cout << "    info                    - информация об арифметике" << endl;
    cout << "    hasse                   - диаграмма хассе" << endl;
    cout << "    tables                  - все таблицы операций" << endl;
    cout << "    add_table               - таблица сложения" << endl;
    cout << "    mul_table               - таблица умножения" << endl;
    cout << "    sub_table               - таблица вычитания" << endl;
    cout << "    div_table               - таблица деления" << endl;
    cout << "    help                    - эта справка" << endl;
    cout << "    exit / quit             - выход" << endl;
    
    cout << "\nпримеры:" << endl;
    cout << "  123 + 456               - сложение положительных" << endl;
    cout << "  -123 + 456              - сложение отрицательного и положительного" << endl;
    cout << "  77777777 + 1            - переполнение (ERR: overflow)" << endl;
    cout << "  -7 / 3                  - деление отрицательного на положительное" << endl;
    cout << "  0 / 0                   - результат: [min;max]" << endl;
    cout << "  5 / 0                   - результат: ∅ (пустое множество)" << endl;
    
    cout << "\nособенности деления с остатком:" << endl;
    cout << "  • при делении -a / b: частное увеличивается на 1, остаток = b - r" << endl;
    cout << "  • остаток может быть многозначным" << endl;
    cout << "  • пример: -7 / 3 = Q: -3 | R: 2  (т.к. -3*3 + 2 = -7)" << endl;
    
    cout << "\n =========================================================== \n" << endl;
}
    
