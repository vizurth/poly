#include "normal.h"

NormalDistribution::NormalDistribution(double mean, double stddev, unsigned int seed) : mean(mean), stddev(stddev), generator(seed), distribution(mean, stddev) {}

// генерация случайного числа по нормальному распределению
double NormalDistribution::generate() {
	double value;
	do {
		value = distribution(generator);
	} while (value <= 0); // гарантируем положительные значения для весов рёбер
	return value;
}

double NormalDistribution::generateAny() {
	return distribution(generator);
}

double NormalDistribution::getMean() const {
	return mean;
}
double NormalDistribution::getStdDev() const {
	return stddev;
}