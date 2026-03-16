#include "weibull.h"

const double WEIBULL_A = 10.0; // масштаб
const double WEIBULL_C = 2.0;  // форма

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
	Генерируем число по распределению Вейбулла
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