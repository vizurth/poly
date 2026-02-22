#pragma once

#include <cmath>
#include <random>

using namespace std;

// класс для генерации случайных чисел по распределению Вейбулла
class WeibullDistribution {
private:
    double a; // параметр масштаба (scale parameter)
    double c; // параметр формы (shape parameter)
    mt19937 generator; // генератор случайных чисел
    uniform_real_distribution<double> uniform; // равномерное распределение для генерации

public:
    // конструктор: принимает параметры масштаба и формы
    WeibullDistribution(double scale = 10.0, double shape = 2.0, unsigned int seed = random_device{}());

    // генерация случайного числа по распределению Вейбулла (всегда положительное)
    double generate();

    // геттеры
    double getScale() const;
    double getShape() const;
};