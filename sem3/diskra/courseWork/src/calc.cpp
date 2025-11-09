#include "../include/calc.hpp"

// Конструктор
FiniteArithmeticCalc::FiniteArithmeticCalc(int n, 
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

// ============ РАБОТА С ДИАГРАММОЙ ХАССЕ БЕЗ INT ============

// Применить plusOne к элементу, пока не вернёмся к стартовому (один полный цикл)
//// Возвращает список всех элементов в порядке обхода
//vector<string> getCycle(const string& start, const map<string, string>& rule) {
//    vector<string> cycle;
//    string current = start;
    
//    do {
//        cycle.push_back(current);
//        current = rule.at(current);
//    } while (current != start);
    
//    return cycle;
//}

//// Применить plusOne N раз через стек (без использования int для вычислений)
//// target - элемент, до которого нужно дойти от start
//string FiniteArithmeticCalc::applyPlusOneNTimes(const string& start, int n) const {
//    if (n < 0) {
//        // Нормализуем отрицательные значения
//        n = n % N;
//        if (n < 0) n += N;
//    }
    
//    n = n % N; // Учитываем цикличность
    
//    string current = start;
    
//    // Используем стек для итерации
//    stack<int> st;
//    st.push(n);
    
//    while (!st.empty()) {
//        int remaining = st.top();
//        st.pop();
        
//        if (remaining == 0) {
//            break;
//        }
        
//        current = plusOneRule.at(current);
        
//        if (remaining > 1) {
//            st.push(remaining - 1);
//        }
//    }
    
//    return current;
//}

//// Применить plusOne до тех пор, пока не достигнем target
//string applyPlusOneUntil(const string& start, const string& target, 
//                         const map<string, string>& rule, int maxSteps) {
//    stack<pair<string, int>> st;
//    st.push({start, 0});
    
//    string current = start;
//    int steps = 0;
    
//    while (steps < maxSteps) {
//        if (current == target) {
//            return current;
//        }
//        current = rule.at(current);
//        steps++;
//    }
    
//    return current;
//}

// Сложение через диаграмму Хассе: a + b
// Начинаем с a и применяем plusOne, пока идём от 0 до b
string FiniteArithmeticCalc::addByHasse(const string& a, const string& b) const {
    // Если b = 0, возвращаем a
    if (b == additiveIdentity) {
        return a;
    }
    
    // Идём от 0 к b, применяя +1 к a каждый раз
    string counterPos = additiveIdentity;  // Счётчик (идёт от 0 к b)
    string result = a;                      // Результат (начинаем с a)
    
    stack<pair<string, string>> st;
    st.push({counterPos, result});
    
    while (!st.empty()) {
        auto [counter, res] = st.top();
        st.pop();
        
        // Если дошли до b, возвращаем результат
        if (counter == b) {
            return res;
        }
        
        // Делаем шаг: counter +1, result +1
        string nextCounter = plusOneRule.at(counter);
        string nextResult = plusOneRule.at(res);
        
        st.push({nextCounter, nextResult});
        result = nextResult;
        
        // Защита от бесконечного цикла
        if (nextCounter == additiveIdentity) {
            // Прошли полный круг, но не нашли b - что-то не так
            break;
        }
    }
    
    return result;
}

// Умножение через диаграмму Хассе: a * b
// Повторное сложение: складываем a с собой, идя от 0 до b
string FiniteArithmeticCalc::multiplyByHasse(const string& a, const string& b) const {
    // Специальный случай: 0 * 0 = универсум
    if (a == additiveIdentity && b == additiveIdentity) {
        return universum;
    }
    
    // Если один из операндов - 0, результат 0
    if (a == additiveIdentity || b == additiveIdentity) {
        return additiveIdentity;
    }
    
    // Используем стек: будем складывать a с собой, двигаясь от 0 к b
    string counter = additiveIdentity;  // Счётчик умножений
    string result = additiveIdentity;    // Накопленная сумма
    
    stack<pair<string, string>> st;
    st.push({counter, result});
    
    while (!st.empty()) {
        auto [cnt, res] = st.top();
        st.pop();
        
        // Если дошли до b, возвращаем результат
        if (cnt == b) {
            return res;
        }
        
        // Делаем шаг: counter +1, result = result + a
        string nextCounter = plusOneRule.at(cnt);
        string nextResult = addByHasse(res, a);
        
        // Защита от бесконечного цикла
        if (nextCounter == additiveIdentity && cnt != additiveIdentity) {
            // Прошли полный круг
            break;
        }
        
        st.push({nextCounter, nextResult});
        result = nextResult;
    }
    
    return result;
}

// Вычитание через диаграмму Хассе: a - b
// Находим x такой, что b + x = a
// Обход через стек
string FiniteArithmeticCalc::subtractByHasse(const string& a, const string& b) const {
    // Перебираем все элементы через стек
    stack<string> st;
    for (const auto& elem : alphabet) {
        st.push(elem);
    }
    
    while (!st.empty()) {
        string candidate = st.top();
        st.pop();
        
        // Проверяем: b + candidate = a ?
        if (addByHasse(b, candidate) == a) {
            return candidate;
        }
    }
    
    return "ERR";
}

// Деление через диаграмму Хассе: a / b
// a / b = a * b^(-1)
string FiniteArithmeticCalc::divideByHasse(const string& a, const string& b) const {
    // Специальный случай: деление на 0 = пустое множество
    if (b == additiveIdentity) {
        return emptySet;
    }
    
    // Проверяем, обратим ли b
    if (inverseMap.find(b) == inverseMap.end()) {
        return "ERR";
    }
    
    // a / b = a * b^(-1)
    string bInverse = inverseMap.at(b);
    return multiplyByHasse(a, bInverse);
}

// ============ ПОСТРОЕНИЕ ТАБЛИЦ ============

// Поиск мультипликативного обратного через перебор
optional<string> FiniteArithmeticCalc::findMultiplicativeInverse(const string& x) {
    if (x == additiveIdentity) {
        return nullopt;
    }
    
    // Перебираем все элементы через стек
    stack<string> st;
    for (const auto& elem : alphabet) {
        st.push(elem);
    }
    
    while (!st.empty()) {
        string candidate = st.top();
        st.pop();
        
        string product = multiplyByHasse(x, candidate);
        if (product == multiplicativeIdentity) {
            return candidate;
        }
    }
    
    return nullopt;
}

void FiniteArithmeticCalc::buildInverseMap() {
    // Используем стек для перебора элементов
    stack<string> st;
    for (const auto& elem : alphabet) {
        st.push(elem);
    }
    
    while (!st.empty()) {
        string elem = st.top();
        st.pop();
        
        auto inv = findMultiplicativeInverse(elem);
        if (inv.has_value()) {
            inverseMap[elem] = inv.value();
        }
    }
}

void FiniteArithmeticCalc::buildAddTable() {
    addTable.assign(N, vector<string>(N, ""));
    
    // Используем стек для обхода таблицы
    stack<pair<int, int>> st;
    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j >= 0; --j) {
            st.push({i, j});
        }
    }
    
    while (!st.empty()) {
        auto [i, j] = st.top();
        st.pop();
        addTable[i][j] = addByHasse(alphabet[i], alphabet[j]);
    }
}

void FiniteArithmeticCalc::buildMulTable() {
    mulTable.assign(N, vector<string>(N, ""));
    
    stack<pair<int, int>> st;
    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j >= 0; --j) {
            st.push({i, j});
        }
    }
    
    while (!st.empty()) {
        auto [i, j] = st.top();
        st.pop();
        mulTable[i][j] = multiplyByHasse(alphabet[i], alphabet[j]);
    }
}

void FiniteArithmeticCalc::buildSubTable() {
    subTable.assign(N, vector<string>(N, ""));
    
    stack<pair<int, int>> st;
    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j >= 0; --j) {
            st.push({i, j});
        }
    }
    
    while (!st.empty()) {
        auto [i, j] = st.top();
        st.pop();
        subTable[i][j] = subtractByHasse(alphabet[i], alphabet[j]);
    }
}

void FiniteArithmeticCalc::buildDivTable() {
    divTable.assign(N, vector<string>(N, ""));
    
    stack<pair<int, int>> st;
    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j >= 0; --j) {
            st.push({i, j});
        }
    }
    
    while (!st.empty()) {
        auto [i, j] = st.top();
        st.pop();
        divTable[i][j] = divideByHasse(alphabet[i], alphabet[j]);
    }
}

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============

int FiniteArithmeticCalc::getIndex(const string& elem) const {
    for (int i = 0; i < N; ++i) {
        if (alphabet[i] == elem) return i;
    }
    return -1;
}

bool FiniteArithmeticCalc::isValidElement(const string& elem) const {
    // Проверяем через стек
    stack<string> st;
    for (const auto& e : alphabet) {
        st.push(e);
    }
    
    while (!st.empty()) {
        if (st.top() == elem) {
            return true;
        }
        st.pop();
    }
    
    return false;
}

// ============ ГЕТТЕРЫ ============

const map<string, string>& FiniteArithmeticCalc::getInverseMap() const {
    return inverseMap;
}

const vector<vector<string>>& FiniteArithmeticCalc::getAddTable() const {
    return addTable;
}

const vector<vector<string>>& FiniteArithmeticCalc::getMulTable() const {
    return mulTable;
}

const vector<vector<string>>& FiniteArithmeticCalc::getSubTable() const {
    return subTable;
}

const vector<vector<string>>& FiniteArithmeticCalc::getDivTable() const {
    return divTable;
}

const vector<string>& FiniteArithmeticCalc::getAlphabet() const {
    return alphabet;
}

// ============ ОПЕРАЦИИ КАЛЬКУЛЯТОРА ============

string FiniteArithmeticCalc::add(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    return addByHasse(a, b);
}

string FiniteArithmeticCalc::multiply(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    return multiplyByHasse(a, b);
}

string FiniteArithmeticCalc::subtract(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    return subtractByHasse(a, b);
}

string FiniteArithmeticCalc::divide(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    return divideByHasse(a, b);
}

// ============ ОБРАТНЫЕ ЭЛЕМЕНТЫ ============

bool FiniteArithmeticCalc::isInvertible(const string& elem) const {
    return inverseMap.find(elem) != inverseMap.end();
}

optional<string> FiniteArithmeticCalc::getInverse(const string& elem) const {
    auto it = inverseMap.find(elem);
    if (it != inverseMap.end()) {
        return it->second;
    }
    return nullopt;
}

// ============ ВЫВОД ТАБЛИЦ И ИНФОРМАЦИИ ============

void FiniteArithmeticCalc::printTable(const vector<vector<string>>& table) const {
    cout << "     ";
    for (const auto& elem : alphabet) {
        cout << elem << "   ";
    }
    cout << endl;
    
    for (int i = 0; i < N; ++i) {
        cout << alphabet[i] << "    ";
        for (int j = 0; j < N; ++j) {
            cout << table[i][j];
            if (table[i][j].length() < 3) {
                cout << "   ";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void FiniteArithmeticCalc::printAddTable() const {
    cout << "\nТаблица сложения (+):" << endl;
    printTable(addTable);
}

void FiniteArithmeticCalc::printMulTable() const {
    cout << "\nТаблица умножения (*):" << endl;
    printTable(mulTable);
}

void FiniteArithmeticCalc::printSubTable() const {
    cout << "\nТаблица вычитания (-):" << endl;
    printTable(subTable);
}

void FiniteArithmeticCalc::printDivTable() const {
    cout << "\nТаблица деления (/):" << endl;
    printTable(divTable);
}

void FiniteArithmeticCalc::printAllTables() const {
    printAddTable();
    printMulTable();
    printSubTable();
    printDivTable();
}

void FiniteArithmeticCalc::printInfo() const {
    cout << "\n=== Информация о конечной арифметике ===" << endl;
    cout << "Размерность: " << N << endl;
    cout << "Аддитивная единица (0): " << additiveIdentity << endl;
    cout << "Мультипликативная единица (1): " << multiplicativeIdentity << endl;
    
    cout << "\nДиаграмма Хассе (правило +1):" << endl;
    for (const auto& [from, to] : plusOneRule) {
        cout << from << " → " << to << endl;
    }
    
    cout << "\nОбратимые элементы:" << endl;
    if (inverseMap.empty()) {
        cout << "Нет обратимых элементов (кроме единицы)" << endl;
    } else {
        for (const auto& [elem, inv] : inverseMap) {
            cout << elem << "^(-1) = " << inv << endl;
        }
    }
    
    cout << "\nСпециальные правила:" << endl;
    cout << "- " << additiveIdentity << " * " << additiveIdentity << " = " << universum << " (универсум)" << endl;
    cout << "- число / " << additiveIdentity << " = " << emptySet << " (пустое множество)" << endl;
}

void FiniteArithmeticCalc::printHelp() const {
    cout << "\n=== ПОМОЩЬ ===" << endl;
    cout << "Доступные команды:" << endl;
    cout << "  <элемент> <операция> <элемент>  - выполнить операцию" << endl;
    cout << "  inv <элемент>                   - найти обратный элемент" << endl;
    cout << "  info                            - показать информацию об арифметике" << endl;
    cout << "  tables                          - показать все таблицы операций" << endl;
    cout << "  add_table                       - показать таблицу сложения" << endl;
    cout << "  mul_table                       - показать таблицу умножения" << endl;
    cout << "  sub_table                       - показать таблицу вычитания" << endl;
    cout << "  div_table                       - показать таблицу деления" << endl;
    cout << "  help                            - показать эту справку" << endl;
    cout << "  exit / quit                     - выход из программы" << endl;
    cout << "\nДоступные операции: + - * /" << endl;
    cout << "Доступные элементы: ";
    for (const auto& elem : alphabet) {
        cout << elem << " ";
    }
    cout << endl;
    cout << "\nСпециальные случаи:" << endl;
    cout << "  " << additiveIdentity << " * " << additiveIdentity << " = " << universum << " (универсум)" << endl;
    cout << "  <число> / " << additiveIdentity << " = " << emptySet << " (пустое множество)" << endl;
    cout << "\nПримеры:" << endl;
    cout << "  b + c" << endl;
    cout << "  h * d" << endl;
    cout << "  inv b" << endl;
}