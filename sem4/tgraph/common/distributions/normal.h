#ifndef NORMAL_H
#define NORMAL_H

#include "distribution.h"
#include <random>

// нормальное (гауссово) распределение
class NormalDistribution : public Distribution {
private:
    double mean;    // среднее значение (μ)
    double stddev;  // стандартное отклонение (σ)
    std::mt19937 generator;  // генератор случайных чисел
    std::normal_distribution<double> distribution;
    
public:
    // конструктор с параметрами распределения
    NormalDistribution(double mean, double stddev);
    
    // генерация случайного числа по нормальному распределению
    double generate() override;
    
    // получить среднее значение
    double getMean() const override;
    
    // получить стандартное отклонение
    double getStdDev() const override;
};

#endif // NORMAL_H
