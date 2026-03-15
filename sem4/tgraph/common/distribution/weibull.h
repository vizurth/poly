#pragma once 
#include <random>
#include <cmath>

class WeibullDistribution {
private:
	double a; // масштаб
	double c; // параметр формы
	std::mt19937 generator; // генератор случайных чисел
	std::uniform_real_distribution<double> distribution; // распределения для генерации 0 до 1

public:
	WeibullDistribution(double scale, double shape);
	WeibullDistribution();
	double generate();
};