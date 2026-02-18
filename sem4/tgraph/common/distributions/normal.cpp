#include "normal.h"

NormalDistribution::NormalDistribution(double mean, double stddev, unsigned int seed) : mean(mean), stddev(stddev), generator(seed), distribution(mean, stddev) {}

/*
	LOOK: double generate()
	Генеринуем положительное число через нормальное распределение
*/
double NormalDistribution::generate() {
	double value;
	do {
		value = distribution(generator);
	} while (value <= 0); // гарантируем положительные значения для весов рёбер
	return value;
}

/*
	LOOK: double generateAny()
	Генеринуем любое число через нормальное распределение
*/
double NormalDistribution::generateAny() {
	return distribution(generator);
}

/*
	LOOK: double getMean()
	Получаем среднее значение
*/
double NormalDistribution::getMean() const {
	return mean;
}

/*
	LOOK: double getStdDev()
	Получаем среднее значение
*/
double NormalDistribution::getStdDev() const {
	return stddev;
}