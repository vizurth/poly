#include "weibull.h"

WeibullDistribution::WeibullDistribution(double scale, double shape, unsigned int seed) 
    : a(scale), c(shape), generator(seed), uniform(0.0, 1.0) {}

/*
    LOOK: double generate()
    Генерируем положительное число через распределение Вейбулла
    Используем метод обратной функции: X = a * (-ln(U))^(1/c), где U ~ Uniform(0,1)
*/
double WeibullDistribution::generate() {
    double u = uniform(generator);
    // избегаем log(0)
    while (u == 0.0 || u == 1.0) {
        u = uniform(generator);
    }
    // формула: X = a * (-ln(U))^(1/c)
    return a * pow(-log(u), 1.0 / c);
}

/*
    LOOK: double getScale()
    Получаем параметр масштаба
*/
double WeibullDistribution::getScale() const {
    return a;
}

/*
    LOOK: double getShape()
    Получаем параметр формы
*/
double WeibullDistribution::getShape() const {
    return c;
}