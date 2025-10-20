#include "../include/Multiset.hpp"
#include "../include/Gray.hpp"

#include <random>
#include <cmath>

Multiset Multiset::Universum;


// заполнение универсума
void Multiset::fillUniverse(int n) {
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

    for (const auto& pair : Universum.getElements()) {
        const string& code = pair.first;
        const int maxCardinality = pair.second;

        while (true) {
            std::cout << "Введите кратность для кода " << code 
                      << " (max: " << maxCardinality << "): ";

            int currentCardinality;
            if (!(cin >> currentCardinality)) {
                cin.clear();
                cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
                cerr << "Ошибка: нужно ввести целое число.\n";
                continue;
            }

            try {
                if (currentCardinality < 0) {
                    throw std::invalid_argument("Ошибка: кратность не может быть отрицательной.");
                }
                if (currentCardinality > maxCardinality) {
                    throw std::out_of_range("Ошибка: кратность не может превышать " + std::to_string(maxCardinality));
                }

                if (currentCardinality > 0) {
                    elements[code] = currentCardinality;
                    totalCardinality += currentCardinality;
                }
                break;

            } catch (const std::exception& e) {
                std::cerr << e.what() << "\n";
            }
        }
    }
}

// автоматическое запонение множеств(A, B)
void Multiset::fillAutomaticly(int desiredCardinality) {
    elements.clear();
    totalCardinality = 0;

    int universeCardinality = Universum.getCardinality();

    if (desiredCardinality < 0 || desiredCardinality > universeCardinality) {
        std::cerr << "Ошибка: желаемая мощность должна быть от 0 до "
                  << universeCardinality << ".\n";
        return;
    }
    if (desiredCardinality == 0) return;

    // получаем список кодов (ключей)
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

    elements = std::move(temp);
    totalCardinality = desiredCardinality;
}

// вывод множества
void Multiset::print(const string& str) const{
	cout << str;
	for (auto& [code, count] : elements)
        cout << code << " : " << count << endl;
	cout << "Общая разрядность: " << totalCardinality << endl;
}

// операции

// обьединение
Multiset Multiset::Union(const Multiset& other) const {
	Multiset result = *this;
    for (auto& [el, cnt] : other.elements)
        result.elements[el] = max(result.elements[el], cnt);
    result.recount();
    return result;
}

// пересечение
Multiset Multiset::Intersection(const Multiset& other) const {
    Multiset result;
    for (auto& [el, cnt] : elements) {
        if (other.elements.count(el))
            result.elements[el] = min(cnt, other.elements.at(el));
    }
    result.recount();
    return result;
}

// дополнение
Multiset Multiset::Complement() const {
    Multiset result;
	map<string, int> Aelem = getElements();
    for (auto& [el, cntU] : Universum.elements) {
        int cntA = Aelem[el];
        if (cntU > cntA)
            result.elements[el] = cntU - cntA;
    }
    result.recount();
    return result;
}

// разность
Multiset Multiset::Diff(const Multiset& other) const {
	Multiset right, result;
	
	right = other.Complement();
	result = this->Intersection(right);

	return result;
}

// симметрическая разность
Multiset Multiset::SimmDiff(const Multiset& other) const{
	Multiset left, right, result;

	left = this->Union(other);
	right = this->Intersection(other);
	result = left.Diff(right);

	return result;
}


// сумма: cnt = min(cntA + cntB, U[el])
Multiset Multiset::operator+(const Multiset& other) const {
    Multiset result;

    for (auto& [el, cntA] : elements) {
        if (Universum.elements.count(el)) {
            int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
            result.elements[el] = min(cntA + cntB, Universum.elements.at(el));
        }
    }

    for (auto& [el, cntB] : other.elements) {
        if (!elements.count(el) && Universum.elements.count(el)) {
            result.elements[el] = min(cntB, Universum.elements.at(el));
        }
    }

    result.recount();
    return result;
}

// разность: cnt = max(0, cntA - cntB)
Multiset Multiset::operator-(const Multiset& other) const {
    Multiset result;

    for (auto& [el, cntA] : elements) {
        if (Universum.elements.count(el)) {
            int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
            result.elements[el] = max(0, cntA - cntB);
        }
    }

    result.recount();
    return result;
}

// произведение: cnt = min(cntA * cntB, U[el])
Multiset Multiset::operator*(const Multiset& other) const {
    Multiset result;

    for (auto& [el, cntA] : elements) {
        if (Universum.elements.count(el) && other.elements.count(el)) {
            int prod = cntA * other.elements.at(el);
            result.elements[el] = min(prod, Universum.elements.at(el));
        }
    }

    result.recount();
    return result;
}

// деление: cnt = min(cntA / cntB, U[el]) если cntB > 0, иначе 0
Multiset Multiset::operator/(const Multiset& other) const {
    Multiset result;

    for (auto& [el, cntA] : elements) {
        if (Universum.elements.count(el)) {
            int cntB = other.elements.count(el) ? other.elements.at(el) : 0;
            int div = (cntB > 0) ? cntA / cntB : 0;
            result.elements[el] = min(div, Universum.elements.at(el));
        }
    }

    result.recount();
    return result;
}
