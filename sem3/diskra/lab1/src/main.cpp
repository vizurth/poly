#include <iostream>
#include <random>

#include <vector>
#include <string>

#include "../include/Gray.hpp"
#include "../include/Multiset.hpp"
#include "../include/UI.hpp"


using namespace std;

/*
	TODO:
	n - разрядность кода грея
	универсум - generateGrayCode(n);
	Заполнение A и B

*/

int main() {
	cout << "Лабораторная работа №1: год Грея\n";

	//int n;
	//cout << "введите кратность (n): ";
	//cin >> n;
	//Multiset::Universum.fillUniverse(n);
	//Multiset::Universum.print("\nуниверсум:\n");

	Multiset A, B;
	UI app(A, B);
	app.run();
}