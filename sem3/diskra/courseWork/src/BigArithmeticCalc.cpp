#include "../include/BigArithmeticCalc.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace {
	// Возвращает:
	// -1, если a < b в циклическом порядке кольца
	//  0, если a == b
	//  1, если a > b
	int compareElementsHelper(const map<string, string>& plusOneRule, int N,
						const string& a, const string& b,
						const string& additiveIdentity) {
		if (a == b) return 0;
		
		// Считаем расстояние от нуля до a
		string current = additiveIdentity;
		int dist_a = 0;
		while (current != a && dist_a < N) {
			current = plusOneRule.at(current);
			dist_a++;
		}
		
		// Считаем расстояние от нуля до b
		current = additiveIdentity;
		int dist_b = 0;
		while (current != b && dist_b < N) {
			current = plusOneRule.at(current);
			dist_b++;
		}
		
		if (dist_a < dist_b) return -1;
		if (dist_a > dist_b) return 1;
	}

	// Проверяет, прошли ли мы хотя бы один полный цикл кольца,
	// начиная от start и двигаясь по plusOneRule
	// Проверяет, замкнётся ли цикл при сложении a + b
	// Проверяет, будет ли оборачивание при сложении a + b в кольце
	bool willWrapAround(const map<string, string>& plusOneRule,
							const string& a, const string& b,
							const string& additiveIdentity) {
		// Прибавление нуля никогда не даёт переноса
		if (b == additiveIdentity) {
			return false;
		}
		
		// Начинаем с нуля: 0 + b где b ∈ {0,1,...,N-1}
		// Результат всегда будет b < N, переноса нет
		if (a == additiveIdentity) {
			return false;
		}
		
		// Симулируем сложение a + b через b шагов по правилу +1
		string current = a;
		string counter = additiveIdentity;
		
		// Делаем ровно b шагов
		while (counter != b) {
			// Шаг 1: увеличиваем счётчик (считаем сколько прибавили)
			counter = plusOneRule.at(counter);
			
			// Шаг 2: увеличиваем текущее значение (a + 1, a + 2, ..., a + b)
			current = plusOneRule.at(current);
			
			// Шаг 3: ПРОВЕРКА ПЕРЕНОСА
			// Если после шага мы оказались в нуле, значит прошли полный цикл
			// (так как начинали НЕ с нуля)
			if (current == additiveIdentity) {
				return true;  // Обёртывание произошло!
			}
		}
		
		// Прошли все b шагов и ни разу не попали в ноль
		// Значит, остались в пределах одного "оборота" диаграммы
		return false;
	}
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

// DONE
string BigArithmeticCalc::addBig(const string& a, const string& b) const { 
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }
    
    string num1 = a;
    string num2 = b;

    // Выравниваем длины
    while (num1.length() < num2.length()) {
        num1 = additiveIdentity + num1;
    }
    while (num2.length() < num1.length()) {
        num2 = additiveIdentity + num2;
    }

    string result;
    string carry = additiveIdentity;

    // Складываем справа налево
    for (int i = static_cast<int>(num1.length()) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        // Шаг 1: складываем два разряда
        string sum = addByHasse(digit1, digit2);
        
        // Шаг 2: добавляем предыдущий перенос
        string sumWithCarry = addByHasse(sum, carry);
        
        // Шаг 3: определяем новый перенос
        // Перенос есть, если:
        // - digit1 + digit2 дал перенос, ИЛИ
        // - sum + carry дал перенос
        bool wrap1 = willWrapAround(plusOneRule, digit1, digit2, additiveIdentity);
        bool wrap2 = willWrapAround(plusOneRule, digit1, digit2, additiveIdentity);
        
        cout << "wrap1(" << digit1 << "+" << digit2 << "):" << wrap1 
             << " wrap2(" << sum << "+" << carry << "):" << wrap2 << endl;
        
        carry = (wrap1 || wrap2) ? multiplicativeIdentity : additiveIdentity;
        
        result = sumWithCarry + result;
        
        cout << digit1 << " + " << digit2 << " + carry:" << carry 
             << " = " << sumWithCarry << endl;
    }

    // Если остался перенос, добавляем старший разряд
    if (carry != additiveIdentity && result.length() < 8) {
        result = carry + result;
    }

    return normalize(result);
}

// DONE
string BigArithmeticCalc::subtractBig(const string& a, const string& b) const {
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }

    // Выравниваем длины чисел
    string num1 = a;
    string num2 = b;
    while (num1.length() < num2.length()) num1 = additiveIdentity + num1;
    while (num2.length() < num1.length()) num2 = additiveIdentity + num2;

    string result;
    string borrow = additiveIdentity;  // 0 в начале

    // Идём справа налево по разрядам
    for (int i = static_cast<int>(num1.length()) - 1; i >= 0; --i) {
        string digit1(1, num1[i]);
        string digit2(1, num2[i]);

        // Учитываем borrow из предыдущего разряда
        if (borrow != additiveIdentity) {
            digit1 = subtractByHasse(digit1, multiplicativeIdentity);
        }

        // Проверяем, нужен ли новый borrow
        int cmp = compareElementsHelper(plusOneRule, N, digit1, digit2, additiveIdentity);
		cout << digit1 << ", " << digit2 << "=" << cmp << endl;
        if (cmp < 0) {
            // Нужен заём из старшего разряда
            borrow = multiplicativeIdentity;
        } else {
            borrow = additiveIdentity;
        }

        // Вычитаем текущий разряд (subtractByHasse корректно делает mod N)
        string diff = subtractByHasse(digit1, digit2);
		cout << digit1 << "-" << digit2 << "=" << diff << "borrow: " << borrow << endl;
        result = diff + result;
    }

    return normalize(result);
}

// DONE
string BigArithmeticCalc::multiplyByDigit(const string& num, const string& digit) const {
    if (digit == additiveIdentity) return additiveIdentity;
    if (digit == multiplicativeIdentity) return num;
    
    string result = additiveIdentity;
    string counter = additiveIdentity;
    
    while (counter != digit) {
		cout << "ADDING " << result << " + " << num << endl;
		cout << "COUNTER: " << counter << endl;
        result = addBig(result, num);
        counter = plusOneRule.at(counter);
    }
    
    return result;
}

string BigArithmeticCalc::multiplyBig(const string& a, const string& b) const {
	cout << "MULTIPLYING " << a << " * " << b << endl;
    if (!isValidNumber(a) || !isValidNumber(b)) {
        return "ERR: Invalid number";
    }
    
    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }
    
    string result = additiveIdentity;
    
    // Умножение столбиком
    for (int i = b.length() - 1; i >= 0; --i) {
        string digit(1, b[i]);
        string partial = multiplyByDigit(a, digit);
		cout << a << "*" << digit << "=" << partial << endl;
        
        // Сдвигаем на позицию
        int shift = b.length() - 1 - i;
        partial = shiftLeft(partial, shift);
		// cout << result << endl;
        result = addBig(result, partial);
        // cout << a << "*" << digit << "=" << partial << " shift: " << shift << " result: " << result << endl;
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
        int cmp = compareElementsHelper(plusOneRule, N, string(1, na[i]), string(1, nb[i]), additiveIdentity);
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
    
    string quotient = additiveIdentity;
    string remainder = a;
    
    // Если делимое меньше делителя
    if (compareBig(a, b) < 0) {
        return {additiveIdentity, a};
    }
    
    // Деление столбиком
    int shift = static_cast<int>(a.length()) - static_cast<int>(b.length());
    string shifted_b = shiftLeft(b, shift);
    
    for (int i = shift; i >= 0; --i) {
        // Ищем цифру частного
        string q_digit = additiveIdentity;
        
        while (compareBig(remainder, shifted_b) >= 0) {
            remainder = subtractBig(remainder, shifted_b);
            q_digit = addByHasse(q_digit, multiplicativeIdentity);
        }
        
        // Добавляем цифру в частное
        if (q_digit != additiveIdentity) {
            quotient = quotient + q_digit;
        } else if (!quotient.empty()) {
            quotient = quotient + additiveIdentity;
        }
        
        // Сдвигаем делитель вправо
        if (i > 0) {
            shifted_b = shiftRight(shifted_b, 1);
        }
    }
    
    if (quotient.empty()) quotient = additiveIdentity;
    
    return {normalize(quotient), normalize(remainder)};
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

// FIXME: я не ебу что тут не так
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
