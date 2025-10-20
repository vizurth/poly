#include "../include/Gray.hpp"
#include "../include/Multiset.hpp"
#include "../include/Input.hpp"

#include <iostream>
#include <random>

static void printMenu() {
	std::cout << "\nМеню:\n";
	std::cout << " 1) Показать код Грея универсума\n";
	std::cout << " 2) Показать множества A и B\n";
	std::cout << " 3) Операции над множествами (объединение, пересечение, разности, симм.разность)\n";
	std::cout << " 4) Дополнения (A^c, B^c по ёмкости) и относительные доп. (A\\B, B\\A)\n";
	std::cout << " 5) Арифметические операции (+, -, *, /) покомпонентно\n";
	std::cout << " 0) Выход\n";
}

static void printVector(const std::vector<int>& v) {
	std::cout << "{";
	for (std::size_t i = 0; i < v.size(); ++i) {
		std::cout << v[i];
		if (i + 1 < v.size()) std::cout << ", ";
	}
	std::cout << "}";
}

int main() {
	std::cout << "Лабораторная работа №1: Мультимножества и код Грея\n";
	const int n = readInt("Введите разрядность кода Грея (1..16): ", 1, 16);

	auto gray = generateGrayCodeStrings(n);
	const std::size_t universeSize = gray.size();

	std::cout << "Размер универсума: " << universeSize << " элементов (по коду Грея).\n";
	const int maxMultiplicity = readInt("Введите максимальную возможную кратность элемента (>=1): ", 1, 1000000);

	Multiset capacity(universeSize, maxMultiplicity);

	// Create A and B
	std::cout << "Выберите способ заполнения множеств A и B:\n";
	std::cout << " 1) Вручную\n 2) Автоматически (случайно)\n";
	int mode = readChoice("Ваш выбор", 2);

	Multiset A(universeSize, 0), B(universeSize, 0);

	if (mode == 1) {
		std::cout << "Ввод A:" << std::endl;
		auto va = readVector("Введите кратности элементов A", universeSize, 0, maxMultiplicity);
		A = Multiset(va);
		std::cout << "Ввод B:" << std::endl;
		auto vb = readVector("Введите кратности элементов B", universeSize, 0, maxMultiplicity);
		B = Multiset(vb);
	} else {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, maxMultiplicity);
		std::vector<int> va(universeSize), vb(universeSize);
		for (std::size_t i = 0; i < universeSize; ++i) { va[i] = dist(gen); }
		for (std::size_t i = 0; i < universeSize; ++i) { vb[i] = dist(gen); }
		A = Multiset(va);
		B = Multiset(vb);
		std::cout << "A и B сгенерированы случайно.\n";
	}

	while (true) {
		printMenu();
		int choice = readChoice("Пункт", 5);
		if (choice == 0) break;
		switch (choice) {
			case 1: {
				std::cout << "Код Грея (" << n << " бит):\n";
				for (std::size_t i = 0; i < gray.size(); ++i) {
					std::cout << i << ": " << gray[i] << "\n";
				}
				break;
			}
			case 2: {
				std::cout << "A = "; A.print(); std::cout << "\n";
				std::cout << "B = "; B.print(); std::cout << "\n";
				break;
			}
			case 3: {
				auto uni = (A | B);
				auto inter = (A & B);
				auto diffAB = (A - B);
				auto diffBA = (B - A);
				auto symm = (A ^ B);
				std::cout << "A  B (объединение) = "; uni.print(); std::cout << "\n";
				std::cout << "A  B (пересечение) = "; inter.print(); std::cout << "\n";
				std::cout << "A \\ B (разность) = "; diffAB.print(); std::cout << "\n";
				std::cout << "B \\ A (разность) = "; diffBA.print(); std::cout << "\n";
				std::cout << "A  B (симм. разность) = "; symm.print(); std::cout << "\n";
				break;
			}
			case 4: {
				auto Ac = A.complement(capacity);
				auto Bc = B.complement(capacity);
				auto AcompB = relative_complement(B, A); // B \ A
				auto BcompA = relative_complement(A, B); // A \ B
				std::cout << "A^c (по ёмкости) = "; Ac.print(); std::cout << "\n";
				std::cout << "B^c (по ёмкости) = "; Bc.print(); std::cout << "\n";
				std::cout << "B \\ A = "; AcompB.print(); std::cout << "\n";
				std::cout << "A \\ B = "; BcompA.print(); std::cout << "\n";
				break;
			}
			case 5: {
				auto s = add(A, B);
				auto d = sub(A, B);
				auto m = mul(A, B);
				auto q = div_euclid(A, B);
				std::cout << "A + B = "; s.print(); std::cout << "\n";
				std::cout << "A - B = "; d.print(); std::cout << "\n";
				std::cout << "A * B = "; m.print(); std::cout << "\n";
				std::cout << "A / B = "; q.print(); std::cout << "\n";
				break;
			}
		}
	}

	std::cout << "До свидания!\n";
	return 0;
} 