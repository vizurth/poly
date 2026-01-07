#include "../include/BigArithmeticCalc.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

BigArithmeticCalc::BigArithmeticCalc(int n, 
                    const map<string, string>& rule, 
                    const vector<string>& alph,
                    const string& addId,
                    const string& mulId)
    : small(n, rule, alph, addId, mulId)
{
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
    if (num == small.getAdditiveIdentity()) return num;
    return negative ? "-" + num : num;
}

// ============ большая арифметика  ============

// проверка не вылезает ли строка за пределы алфавита
bool BigArithmeticCalc::isValidNumber(const string& num) const {
    if (num.empty()) return false;
    
    string unsign_num = removeSign(num);
    
    for (char c : unsign_num) {
        if (!small.isValidElement(string(1, c))) return false;
    }
    return true;
}

// проверка на переполнение по количеству разрядов
bool BigArithmeticCalc::isOverflow(const string& num) const {
    string unsign_num = removeSign(num);
    string withoutZeros = deleteTrashZeros(unsign_num);
    return withoutZeros.length() > static_cast<size_t>(small.getMaxDigits());
}

/*
	LOOK: deleteTrashZeros(num) 
	функция для удаление ведущих нулей
	так как мы можем при вычислении добавить ведущие нули для упрощения вычислений
	бедем счетчиком считать количество нулей спереди числа а далее просто обрежем их
*/
string BigArithmeticCalc::deleteTrashZeros(const string& num) const {
    if (num.empty()) return small.getAdditiveIdentity();
    
    bool negative = isNegative(num);
    string unsign_num = removeSign(num);
    
    // удаляем ведущие нули
    size_t firstNonZero = 0;
    while (firstNonZero < unsign_num.length() && 
           string(1, unsign_num[firstNonZero]) == small.getAdditiveIdentity()) {
        firstNonZero++;
    }
    
    if (firstNonZero == unsign_num.length()) {
        return small.getAdditiveIdentity();
    }
    
    string result = unsign_num.substr(firstNonZero);
    
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
        num1 = small.getAdditiveIdentity() + num1;
    }
    while (num2.length() < max_len) {
        num2 = small.getAdditiveIdentity() + num2;
    }

    string result;
    string carry = small.getAdditiveIdentity();

    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        auto res = small.addWithCarry(digit1, digit2, carry);
        if (res.first == "ERR") {
            return "ERR: invalid addition";
        }
        
        string sum = res.first;
        carry = res.second;
        
        result = sum + result;
    }

    if (carry != small.getAdditiveIdentity()) {
        result = carry + result;
    }

    if (isOverflow(result)) {
        return "ERR: overflow";
    }

    return deleteTrashZeros(result);
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
        return small.getAdditiveIdentity();
    }
    
    // определяем большее и меньшее число
    string larger = (cmp >= 0) ? a : b;
    string smaller = (cmp >= 0) ? b : a;

    // выравниваем длины
    size_t max_len = max(larger.length(), smaller.length());
    while (larger.length() < max_len) {
        larger = small.getAdditiveIdentity() + larger;
    }
    while (smaller.length() < max_len) {
        smaller = small.getAdditiveIdentity() + smaller;
    }

    string result;
    bool borrow = false; // флаг заема

    // вычитаем справа налево
    for (int i = static_cast<int>(max_len) - 1; i >= 0; --i) {
        string current(1, larger[i]);
        string digit2(1, smaller[i]);
        
        if (borrow) {
			if (current == small.getAdditiveIdentity()) {
                current = small.getAlphabet().back();
            } else {
                current = small.smallSubtract(current, small.getMultiplicativeIdentity());
                borrow = false;
            }
        }
        
        int cmp_digits = small.compareElems(current, digit2);
        
        if (cmp_digits >= 0) {
            string result_digit = small.smallSubtract(current, digit2);
            result = result_digit + result;
        } else {
            string temp = current; // аналог 10 + a (если нужem заем)
            for (int j = 0; j < small.getN(); j++) {
                temp = small.nextElem(temp);
            }

            // теперь вычетаем
            string result_digit = small.smallSubtract(temp, digit2);
            result = result_digit + result;
            borrow = true; // флаг
        }
    }
    if (borrow) {
        return "ERR: borrow after subtraction";
    }
    return deleteTrashZeros(result);
}

/*
	LOOK: multiplyByDigit(num, digit)
	функция умножения числа на цифру
	просто будем использовать counter чтолбы считать сколько раз уже умножили 
	и result = addBigUnsigned(result, num)
*/
string BigArithmeticCalc::multiplyByDigit(const string& num, const string& digit) const {
    if (digit == small.getAdditiveIdentity()) return small.getAdditiveIdentity();
    if (digit == small.getMultiplicativeIdentity()) return num;
    
    string result = small.getAdditiveIdentity();
    string counter = small.getAdditiveIdentity();
    
    // умножаем через многократное сложение с буквенным счетчиком
    while (counter != digit) {
        result = addBigUnsigned(result, num);
        if (result.substr(0, 4) == "ERR:") return result;
        counter = small.nextElem(counter);
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
    
    if (a == small.getAdditiveIdentity() || b == small.getAdditiveIdentity()) {
        return "[" + getMinNumber() + ";" + getMaxNumber() + "]";
    }
    
    string result = small.getAdditiveIdentity();
    
    // умножение столбиком: проходим по каждой цифре множителя справа налево
    for (int i = b.length() - 1; i >= 0; --i) {
        string multiplier_digit(1, b[i]);
        
        if (multiplier_digit != small.getAdditiveIdentity()) {
            string partial_product = multiplyByDigit(a, multiplier_digit);
            if (partial_product.substr(0, 4) == "ERR:") return partial_product;
            
            // сдвиг влево (добавляем нули справа)
            int shift_count = b.length() - 1 - i;
            for (int j = 0; j < shift_count; j++) {
                partial_product += small.getAdditiveIdentity();
            }
            
            result = addBigUnsigned(result, partial_product);
            if (result.substr(0, 4) == "ERR:") return result;
        }
    }
    
    return deleteTrashZeros(result);
}

/*
	LOOK: compareBigUnsigned(a, b)
	функция для сравнения беззнаковых больших чисел
	сравниваем по длине а далее поразрядно слева направо c помощью compareSymbols
*/
int BigArithmeticCalc::compareBigUnsigned(const string& a, const string& b) const {
    string na = deleteTrashZeros(a);
    string nb = deleteTrashZeros(b);
    
    if (na.length() != nb.length()) {
        return na.length() > nb.length() ? 1 : -1;
    }
    
    for (size_t i = 0; i < na.length(); ++i) {
        int cmp = small.compareElems(string(1, na[i]), string(1, nb[i]));
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
    if (b == small.getAdditiveIdentity()) {
        // 0/0 = [min;max]
        if (a == small.getAdditiveIdentity()) {
            return {small.getUniversum(), small.getUniversum()};
        }
        return {small.getEmptySet(), small.getEmptySet()};
    }
    
    // если делимое равно нулю
    if (a == small.getAdditiveIdentity()) {
        return {small.getAdditiveIdentity(), small.getAdditiveIdentity()};
    }
    
    string dividend = a;
    string divisor = b;
    
    // если делимое меньше делителя
    if (compareBigUnsigned(dividend, divisor) < 0) {
        return {small.getAdditiveIdentity(), dividend};
    }
    
    // деление в столбик
    string q_str;
    string curr_r = small.getAdditiveIdentity();
    
    // проходим по каждой цифре делимого слева направо
    for (size_t i = 0; i < dividend.length(); i++) {
        // добавляем следующую цифру к остатку
        curr_r = curr_r + string(1, dividend[i]);
        curr_r = deleteTrashZeros(curr_r);
        
        // ищем максимальную цифру частного
        string q_digit = small.getAdditiveIdentity();
        
        // пробуем все возможные цифры от 1 до максимальной
        string test_digit = small.getMultiplicativeIdentity();
        while (test_digit != small.getAdditiveIdentity()) {
            string test_product = multiplyByDigit(divisor, test_digit);
            
            if (compareBigUnsigned(test_product, curr_r) > 0) {
                break;
            }
            
            q_digit = test_digit;
            test_digit = small.nextElem(test_digit);
        }
        
        q_str += q_digit;
        
        // вычитаем произведение из остатка
        if (q_digit != small.getAdditiveIdentity()) {
            string product = multiplyByDigit(divisor, q_digit);
            curr_r = subtractBigUnsigned(curr_r, product);
        }
    }
    
    string q = deleteTrashZeros(q_str);
    string r = deleteTrashZeros(curr_r);
    
    return {q, r};
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
    
    string unsign_a = removeSign(a);
    string unsign_b = removeSign(b);
    
    // a + b (оба положительные)
    if (!neg_a && !neg_b) {
        return addBigUnsigned(unsign_a, unsign_b);
    }
    
    // -a + (-b) = -(a + b)
    if (neg_a && neg_b) {
        string sum = addBigUnsigned(unsign_a, unsign_b);
        if (sum.substr(0, 4) == "ERR:") return sum;
        return addSign(sum, true);
    }
    
    // a + (-b) = a - b
    if (!neg_a && neg_b) {
        int cmp = compareBigUnsigned(unsign_a, unsign_b);
        if (cmp >= 0) {
            return subtractBigUnsigned(unsign_a, unsign_b);
        } else {
            string diff = subtractBigUnsigned(unsign_b, unsign_a);
			cout << "Debug:" << diff << endl;
            return addSign(diff, true);
        }
    }
    
    // -a + b = b - a
    if (neg_a && !neg_b) {
        int cmp = compareBigUnsigned(unsign_b, unsign_a);
        if (cmp >= 0) {
            return subtractBigUnsigned(unsign_b, unsign_a);
        } else {
            string diff = subtractBigUnsigned(unsign_a, unsign_b);
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
    bool neg_a = isNegative(a);
    bool neg_b = isNegative(b);

    string ua = removeSign(a);
    string ub = removeSign(b);

    //  a - b  (оба ≥ 0)
    if (!neg_a && !neg_b) {
        int cmp = compareBigUnsigned(ua, ub);
        if (cmp >= 0) return subtractBigUnsigned(ua, ub);
        string diff = subtractBigUnsigned(ub, ua);
        return addSign(diff, true); // отрицательный результат
    }

    //  a - (-b) = a + b
    if (!neg_a && neg_b) {
        return addBigUnsigned(ua, ub);
    }

    // -a - b = -(a + b)
    if (neg_a && !neg_b) {
        string sum = addBigUnsigned(ua, ub);
        if (sum.substr(0, 4) == "ERR:") return sum;
        return addSign(sum, true);
    }

    // -a - (-b) = b - a
    if (neg_a && neg_b) {
        int cmp = compareBigUnsigned(ua, ub);
        if (cmp > 0) {
            string diff = subtractBigUnsigned(ua, ub);
            return addSign(diff, true);   // -(a - b)
        } else if (cmp < 0) {
            string diff = subtractBigUnsigned(ub, ua);
            return diff;
        }
        return small.getAdditiveIdentity();
    }

    return "ERR: unreachable case subtract()";
}

/*
	LOOK: multiply(a, b)
	функция для умножения больших чисел со знаком
	результат отрицательный, если знаки разные
*/
string BigArithmeticCalc::multiply(const string& a, const string& b) const {
    bool neg_a = isNegative(a);
    bool neg_b = isNegative(b);
    
    string unsign_a = removeSign(a);
    string unsign_b = removeSign(b);
    
    string product = multiplyBigUnsigned(unsign_a, unsign_b);
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

    string unsign_a = removeSign(a);
    string unsign_b = removeSign(b);
    
    auto [q, r] = divideBigUnsigned(unsign_a, unsign_b);
    
    // обработка специальных случаев
    if (q == small.getEmptySet() || q == small.getUniversum()) {
        return "Q: " + q + " | R: " + r;
    }
    
    if (q.substr(0, 3) == "ERR") {
        return q;
    }
    
    // если делимое отрицательное, а делитель положительный
    // то -a / b = -(a/b + 1), остаток = b - r
    if (neg_a && !neg_b && r != small.getAdditiveIdentity()) {
        // добавляем единицу к частному
        q = addBigUnsigned(q, small.getMultiplicativeIdentity());
        // вычисляем новый остаток: b - r
        r = subtractBigUnsigned(unsign_b, r);
        q = addSign(q, true);
    }
    // если делимое положитmельное, а делитель отрицательный
    // то a / (-b) = -(a/b), остаток остается r
    else if (!neg_a && neg_b) {
        q = addSign(q, true);
    } else if (neg_a && !neg_b) { // -a / b = -(a/b) без остатка
        q = addSign(q, true);
    }
    // если оба отрицательные: -a / (-b) = a/b
    else if (neg_a && neg_b) {}
    
    return "Q: " + q + " | R: " + r;
}

// ============ геттеры ============

const vector<string>& BigArithmeticCalc::getAlphabet() const {
    return small.getAlphabet();
}

string BigArithmeticCalc::getMinNumber() const {
    return small.getMinNumber();
}

string BigArithmeticCalc::getMaxNumber() const {
    return small.getMaxNumber();
}

// ============ вывод менюшек ============

void BigArithmeticCalc::printAddTable() const {
    small.printAddTable();
}

void BigArithmeticCalc::printMulTable() const {
    small.printMulTable();
}

void BigArithmeticCalc::printSubTable() const {
    small.printSubTable();
}

void BigArithmeticCalc::printDivTable() const {
    small.printDivTable();
}

void BigArithmeticCalc::printAllTables() const {
    small.printAllTables();
}

void BigArithmeticCalc::printHasseDiagram() const {
    small.printHasseDiagram();
}

void BigArithmeticCalc::printInfo() const {
    cout << "=========== ИНФОРМАЦИЯ О КОНЕЧНОЙ АРИФМЕТИКЕ ===========" << endl;
    
    cout << "\nразмерность: Z" << small.getN() << endl;
    cout << "аддитивная единица (0): '" << small.getAdditiveIdentity() << "'" << endl;
    cout << "мультипликативная единица (1): '" << small.getMultiplicativeIdentity() << "'" << endl;
    cout << "\nмаксимальное количество разрядов: " << small.getMaxDigits() << endl;
    cout << "диапазон чисел: [" << getMinNumber() << "; " << getMaxNumber() << "]" << endl;
    
    cout << "\nспециальные правила:" << endl;
    cout << "  • " << small.getAdditiveIdentity() << " * " << small.getAdditiveIdentity() 
         << " = " << small.getUniversum() << " (универсум)" << endl;
    cout << "  • число / " << small.getAdditiveIdentity() << " = " << small.getEmptySet() 
         << " (пустое множество)" << endl;
    cout << "  • " << small.getAdditiveIdentity() << " / " << small.getAdditiveIdentity() 
         << " = " << small.getUniversum() << " (диапазон всех чисел)" << endl;
    cout << "  • переполнение (> " << small.getMaxDigits() << " разрядов) = ERR: overflow" << endl;
}

void BigArithmeticCalc::printHelp() const {
    cout << "=========== ПОМОЩЬ ===========" << endl;
    
    cout << "\nдоступные команды:\n" << endl;
    cout << "\n  БОЛЬШАЯ АРИФМЕТИКА (до " << small.getMaxDigits() << " разрядов, с отрицательными):" << endl;
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
    cout << "  abd + ghf               - сложение положительных" << endl;
    cout << "  -abd + ghf              - сложение отрицательного и положительного" << endl;
    cout << "  hhhhhhhh + b            - переполнение (ERR: overflow)" << endl;
    cout << "  -c / d                  - деление отрицательного на положительное" << endl;
    cout << "  a / a                   - результат:" << "[" << getMinNumber() << "; " << getMaxNumber() << "]" << endl;
    cout << "  g / a                   - результат: ∅ (пустое множество)" << endl;
    
    cout << "\nособенности деления с остатком:" << endl;
    cout << "  • при делении -a / b: частное увеличивается на 1, остаток = b - r" << endl;
    cout << "  • остаток может быть многозначным" << endl;
    
    cout << "\n =========================================================== \n" << endl;
}
    
