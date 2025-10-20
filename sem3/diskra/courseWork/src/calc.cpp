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
    buildHasseVector();
    buildHasseWeight();
    buildInverseMap();
    buildAddTable();
    buildMulTable();
    buildSubTable();
    buildDivTable();
}

// Приватные методы инициализации
void FiniteArithmeticCalc::buildHasseVector() {
    hasseVector.resize(N);
    hasseVector[0] = additiveIdentity;
    
    for (int i = 1; i < N; ++i) {
        hasseVector[i] = plusOneRule.at(hasseVector[i-1]);
    }
}

void FiniteArithmeticCalc::buildHasseWeight() {
    for (int i = 0; i < N; ++i) {
        hasseWeight[hasseVector[i]] = i;
    }
}

optional<string> FiniteArithmeticCalc::findMultiplicativeInverse(const string& x) {
    int xWeight = hasseWeight.at(x);
    
    if (xWeight == 0) {
        return nullopt;
    }
    
    for (int i = 0; i < N; ++i) {
        if ((xWeight * i) % N == 1) {
            return hasseVector[i];
        }
    }
    
    return nullopt;
}

void FiniteArithmeticCalc::buildInverseMap() {
    for (const auto& elem : alphabet) {
        auto inv = findMultiplicativeInverse(elem);
        if (inv.has_value()) {
            inverseMap[elem] = inv.value();
        }
    }
}

void FiniteArithmeticCalc::buildAddTable() {
    addTable.assign(N, vector<string>(N, ""));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int indexHas = (hasseWeight.at(alphabet[i]) + hasseWeight.at(alphabet[j])) % N;
            addTable[i][j] = hasseVector[indexHas];
        }
    }
}

void FiniteArithmeticCalc::buildMulTable() {
    mulTable.assign(N, vector<string>(N, ""));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int indexHas = (hasseWeight.at(alphabet[i]) * hasseWeight.at(alphabet[j])) % N;
            mulTable[i][j] = hasseVector[indexHas];
        }
    }
}

void FiniteArithmeticCalc::buildSubTable() {
    subTable.assign(N, vector<string>(N, ""));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int indexHas = (hasseWeight.at(alphabet[i]) - hasseWeight.at(alphabet[j]) + N) % N;
            subTable[i][j] = hasseVector[indexHas];
        }
    }
}

void FiniteArithmeticCalc::buildDivTable() {
    divTable.assign(N, vector<string>(N, ""));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (inverseMap.find(alphabet[j]) == inverseMap.end()) {
                divTable[i][j] = "ERR";
            } else {
                int dividendWeight = hasseWeight.at(alphabet[i]);
                int inverseWeight = hasseWeight.at(inverseMap.at(alphabet[j]));
                int resultWeight = (dividendWeight * inverseWeight) % N;
                divTable[i][j] = hasseVector[resultWeight];
            }
        }
    }
}

int FiniteArithmeticCalc::getIndex(const string& elem) const {
    for (int i = 0; i < N; ++i) {
        if (alphabet[i] == elem) return i;
    }
    return -1;
}

bool FiniteArithmeticCalc::isValidElement(const string& elem) const {
    return getIndex(elem) != -1;
}

// Геттеры

const vector<string>& FiniteArithmeticCalc::getHasseVector() const {
    return hasseVector;
}

const map<string, int>& FiniteArithmeticCalc::getHasseWeight() const {
    return hasseWeight;
}

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

// Операции калькулятора

string FiniteArithmeticCalc::add(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    
    int i = getIndex(a);
    int j = getIndex(b);
    return addTable[i][j];
}

string FiniteArithmeticCalc::multiply(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    
    // Специальный случай: 0 * 0 = универсум
    if (a == additiveIdentity && b == additiveIdentity) {
        return universum;
    }
    
    int i = getIndex(a);
    int j = getIndex(b);
    return mulTable[i][j];
}

string FiniteArithmeticCalc::subtract(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    
    int i = getIndex(a);
    int j = getIndex(b);
    return subTable[i][j];
}

string FiniteArithmeticCalc::divide(const string& a, const string& b) const {
    if (!isValidElement(a) || !isValidElement(b)) return "ERR: Недопустимый элемент";
    
    // Специальный случай: деление на 0 = пустое множество
    if (b == additiveIdentity) {
        return emptySet;
    }
    
    int i = getIndex(a);
    int j = getIndex(b);
    return divTable[i][j];
}

// Проверка и получение обратного элемента

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

// Вывод таблиц и информации
void FiniteArithmeticCalc::printTable(const vector<vector<string>>& table) const {
    // Заголовок столбцов
    cout << "   ";
    for (const auto& elem : alphabet) {
        cout << elem << " ";
    }
    cout << endl;
    
    // Строки таблицы
    for (int i = 0; i < N; ++i) {
        cout << alphabet[i] << "  ";
        for (int j = 0; j < N; ++j) {
            cout << table[i][j] << " ";
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
    
    cout << "\nHasse диаграмма: ";
    for (const auto& elem : hasseVector) {
        cout << elem << " ";
    }
    cout << endl;
    
    cout << "\nВеса элементов:" << endl;
    for (const auto& [elem, weight] : hasseWeight) {
        cout << elem << " : " << weight << endl;
    }
    
    cout << "\nОбратимые элементы:" << endl;
    for (const auto& [elem, inv] : inverseMap) {
        cout << elem << " ^ -1 = " << inv << endl;
    }
    
    cout << "\nСпециальные правила:" << endl;
    cout << "- 0 * 0 = " << universum << " (универсум)" << endl;
    cout << "- число / 0 = " << emptySet << " (пустое множество)" << endl;
}

void FiniteArithmeticCalc::printHelp() const {
    cout << "\n=== ПОМОЩЬ ===" << endl;
    cout << "Доступные команды:" << endl;
    cout << "  <элемент> <операция> <элемент>  - выполнить операцию" << endl;
    cout << "  <элемент> ^ <число>             - возведение в степень" << endl;
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
}