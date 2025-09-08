#pragma once

#include "Shape.h"
#include "MyVector.h"

class Circle : public Shape {
private:
	Vector ptCntr;
	double R;
public:
	Circle(double cx, double cy, double r);
	Circle(const Vector& center, double r);
	~Circle() override = default;

	void Move(Vector& v) override;
	void Out() const override;
	double Area() const override;
}; 