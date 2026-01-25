#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

// базовый абстрактный класс для всех распределений
class Distribution {
public:
    virtual ~Distribution() = default;
    
    // генерация случайного числа по распределению
    virtual double generate() = 0;
    
    // получить среднее значение распределения
    virtual double getMean() const = 0;
    
    // получить стандартное отклонение распределения
    virtual double getStdDev() const = 0;
};

#endif // DISTRIBUTION_H
