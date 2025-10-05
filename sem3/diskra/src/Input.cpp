#include "../include/Input.hpp"

#include <iostream>
#include <limits>

static void clearLine() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
	while (true) {
		std::cout << prompt;
		long long x;
		if (std::cin >> x) {
			clearLine();
			if (x >= minValue && x <= maxValue) return static_cast<int>(x);
			std::cout << "Введите число в диапазоне [" << minValue << ", " << maxValue << "]\n";
		} else {
			std::cout << "Некорректный ввод. Попробуйте ещё раз.\n";
			clearLine();
		}
	}
}

std::vector<int> readVector(const std::string& prompt, std::size_t size, int minValue, int maxValue) {
	std::vector<int> v(size, 0);
	std::cout << prompt << " (" << size << "):\n";
	for (std::size_t i = 0; i < size; ++i) {
		v[i] = readInt("  элемент[" + std::to_string(i) + "]: ", minValue, maxValue);
	}
	return v;
}

int readChoice(const std::string& prompt, int k) {
	return readInt(prompt + " (1.." + std::to_string(k) + "): ", 1, k);
} 