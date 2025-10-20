#pragma once

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Multiset {
private:
	map<string, int> elements;
	int totalCardinality = 0;
	void recount() {
        totalCardinality = 0;
        for (auto& [_, cnt] : elements)
            totalCardinality += cnt;
    }

public:
	static Multiset Universum;

	Multiset() : totalCardinality(0) {}
	int getCardinality() const { return totalCardinality; }
	const map<string, int>& getElements() const { return elements; }

	// заполнение универсума
	void fillUniverse(int n);
	// ручное заполнение множеств(A, B)
	void fillByHand();
	// автоматическое запонение множеств(A, B)
	void fillAutomaticly(int n);

	// проверка на пустоту
	bool isEmpty() const { return elements.size() == 0; }

	// вывод множества
	void print(const string& str) const;

	// обьединение
	Multiset Union(const Multiset& other) const;
	// пересечение
	Multiset Intersection(const Multiset& other) const;
	// дополнение
	Multiset Complement() const;
	// разность
	Multiset Diff(const Multiset& other) const;
	// симметрическая разность
	Multiset SimmDiff(const Multiset& other) const;

	// Арифменические операции
	Multiset operator+(const Multiset& other) const;
    Multiset operator-(const Multiset& other) const;
    Multiset operator*(const Multiset& other) const;
    Multiset operator/(const Multiset& other) const;

};
