#include "weibull.h"

// в дальнейшем подберем параметры для генерации весов и структуры графа
const double WEIBULL_A = 10.0; // масштаб (a)
const double WEIBULL_C = 2.0;  // форма (c)

/*
	LOOK: WeibullDistribution(double scale, double shape)
	Конструктор класса
*/
WeibullDistribution::WeibullDistribution(double scale, double shape) 
	: a(scale), c(shape), generator(std::random_device{}()), distribution(0.0, 1.0) {}


/*
	LOOK: WeibullDistribution()
	Конструктор класса с параметрами по умолчанию
*/
WeibullDistribution::WeibullDistribution() 
	: a(WEIBULL_A), c(WEIBULL_C), generator(std::random_device{}()), distribution(0.0, 1.0) {}


/*
	LOOK: double generate()
	Генерируем число по распределению Вейбулла с помощью inverse transform method
*/
double WeibullDistribution::generate() {
	double u = distribution(generator);
	double raw = a * std::pow(-std::log(1 - u), 1.0 / c);
	if (raw > 0) {
		return raw;
	} else {
		return -raw;
	}
}