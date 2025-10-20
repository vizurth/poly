#include "../include/Multiset.hpp"
#include "../include/Gray.hpp"

#include <random>
#include <cmath>
#include <limits>

Multiset Multiset::Universum;

// заполнение универсума
void Multiset::fillUniverse(int n) {
    elements.clear();
    totalCardinality = 0;

    if (n <= 0) {
        cout << "Создан пустой универсум (разрядность 0)\n";
        return;
    }

    vector<string> grayCodes = generateGrayCode(n);
    random_device rb;
    mt19937 gen(rb());
    uniform_int_distribution<> dist(1, 50);

    for (const auto& code : grayCodes) {
        int car = dist(gen);
        elements[code] = car;
        totalCardinality += car;
    }
}

// ручное заполнение множеств(A, B)
void Multiset::fillByHand() {
    elements.clear();
    totalCardinality = 0;

    if (Universum.isEmpty()) {
        cout << "Универсум пуст — множество будет пустым.\n";
        return;
    }

    for (const auto& pair : Universum.getElements()) {
        const string& code = pair.first;
        const int maxCardinality = pair.second;

        while (true) {
            cout << "Введите кратность для кода " << code
                 << " (max: " << maxCardinality << "): ";

            int currentCardinality;
            if (!(cin >> currentCardinality)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Ошибка: нужно ввести целое число.\n";
                continue;
            }

            try {
                if (currentCardinality < 0) {
                    throw invalid_argument("Ошибка: кратность не может быть отрицательной.");
                }
                if (currentCardinality > maxCardinality) {
                    throw out_of_range("Ошибка: кратность не может превышать " + to_string(maxCardinality));
                }

                if (currentCardinality > 0) {
                    elements[code] = currentCardinality;
                    totalCardinality += currentCardinality;
                }
                break;

            } catch (const exception& e) {
                cerr << e.what() << "\n";
            }
        }
    }
}

// автоматическое заполнение множеств(A, B)
void Multiset::fillAutomaticly(int desiredCardinality) {
    elements.clear();
    totalCardinality = 0;

    if (Universum.isEmpty()) {
        cout << "Универсум пуст — множество останется пустым.\n";
        return;
    }

    int universeCardinality = Universum.getCardinality();

    if (desiredCardinality < 0 || desiredCardinality > universeCardinality) {
        cerr << "Ошибка: мощность должна быть от 0 до "
             << universeCardinality << ".\n";
        return;
    }

    if (desiredCardinality == 0) {
        cout << "Создано пустое множество.\n";
        return;
    }

    vector<string> codes;
    for (const auto& [code, _] : Universum.getElements()) {
        codes.push_back(code);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, (int)codes.size() - 1);
    map<string, int> temp;

    int added = 0;
    while (added < desiredCardinality) {
        const string& code = codes[dist(gen)];
        if (temp[code] < Universum.getElements().at(code)) {
            temp[code]++;
            ++added;
        }
    }

    elements = move(temp);
    totalCardinality = desiredCardinality;
}

// вывод множества
void Multiset::print(const string& str) const {
    cout << str;
    if (elements.empty()) {
        cout << "(множество пусто)\n";
        return;
    }

    for (auto& [code, count] : elements)
        cout << code << " : " << count << endl;

    cout << "Общая мощность: " << totalCardinality << endl;
}

// объединение
Multiset Multiset::Union(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = max(cntA, cntB);
    }
    result.recount();
    return result;
}

// пересечение
Multiset Multiset::Intersection(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = min(cntA, cntB);
    }
    result.recount();
    return result;
}

// дополнение
Multiset Multiset::Complement() const {
    Multiset result;
    for (auto& [el, cntU] : Universum.elements) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        result.elements[el] = max(0, cntU - cntA);
    }
    result.recount();
    return result;
}

// разность
Multiset Multiset::Diff(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = max(0, cntA - cntB);
    }
    result.recount();
    return result;
}

// симметрическая разность
Multiset Multiset::SimmDiff(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = abs(cntA - cntB);
    }
    result.recount();
    return result;
}

// арифметические операции

Multiset Multiset::operator+(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = min(cntA + cntB, cntU);
    }
    result.recount();
    return result;
}

Multiset Multiset::operator-(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = max(0, cntA - cntB);
    }
    result.recount();
    return result;
}

Multiset Multiset::operator*(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        result.elements[el] = min(cntA * cntB, cntU);
    }
    result.recount();
    return result;
}

Multiset Multiset::operator/(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cntU] : Universum.getElements()) {
        int cntA = elements.count(el) ? elements.at(el) : 0;
        int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
        int div = (cntB > 0) ? (cntA / cntB) : 0;
        result.elements[el] = min(div, cntU);
    }
    result.recount();
    return result;
}
