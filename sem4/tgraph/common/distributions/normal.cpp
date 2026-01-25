#include "normal.h"
#include <chrono>

// конструктор: инициализируем параметры распределения и генератор случайных чисел
NormalDistribution::NormalDistribution(double mean, double stddev) 
    : mean(mean), stddev(stddev) {
    // инициализируем генератор текущим временем для разных последовательностей при каждом запуске
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::mt19937(seed);
    distribution = std::normal_distribution<double>(mean, stddev);
}

// генерация случайного числа по нормальному распределению
double NormalDistribution::generate() {
    return distribution(generator);
}

// получить среднее значение распределения
double NormalDistribution::getMean() const {
    return mean;
}

// получить стандартное отклонение
double NormalDistribution::getStdDev() const {
    return stddev;
}
