#pragma once

#include "Shape.h"

class Vector: public Shape{
private:
	double x, y;	// Координаты вектора на плоскости
	static int Count;
public:
	//========== Три конструктора
	Vector (double c1, double c2);
	Vector ();						// Default
	Vector(const Vector& other); // конструктор копирования

	~Vector();
	
	//====== Переопределение операций =====//
	Vector& operator= (const Vector& other);	// конструктор присваивания
	Vector operator+ (const Vector& other);

	Vector operator* (double scalar) const;
	friend Vector operator*(double scalar, const Vector& vec);
	friend double operator*(const Vector& a, const Vector& b);

	double operator!() const;
	bool operator>(const Vector& other) const;
	bool operator==(const Vector& other) const;
	void Out() const override;

	void Move(Vector& v) override;
	double Area() const override;

	double GetX() const { return x; }
	double GetY() const { return y; }

	static void PrintCount();
};