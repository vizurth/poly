#pragma once

#include "MyVector.h"
#include "Shape.h"

class Rect : public Shape {
private:
	Vector ptLT_;
	Vector ptRB_;
public:
	Rect(); //default
	Rect(const Rect& other); // конструктор копирования
	Rect(double left, double top, double right, double bottom);
	Rect(const Vector& lt, const Vector& rb);

	Rect& operator=(const Rect& other);
	
	void Inflate(double value = 1.0);
	void Inflate(double x_c, double y_c);
	void Inflate(double left, double top, double right, double bottom);

	void Out() const override;
	void Move(Vector& v) override;
	double Area() const override;
};