#pragma once

#include <cmath>
#include <random>

// класс для генерации случайных чисел по нормальному распределению

class NormalDistribution {
private:
	double mean; // среднее значение (μ)
	double stddev; // стандартное отклонение (σ)
	std::mt19937 generator; // генератор случайных чисел
	std::normal_distribution<double> distribution; // распределение для генерации чисел

public:
	// конструктор: принимает среднее и стандартное отклонение
	NormalDistribution(double mean = 10.0, double stddev = 3.0, unsigned int seed = std::random_device{}());

	// генерация случайного числа по нормальному распределению
	double generate();

	double generateAny();

	double getMean() const;
	double getStdDev() const;
};