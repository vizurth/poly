#include "headers/stdafx.h"
#include "headers/Circle.h"

Circle::Circle(double cx, double cy, double r) : ptCntr(cx, cy), R(r) {}

Circle::Circle(const Vector& center, double r) : ptCntr(center), R(r) {}

void Circle::Move(Vector& v){
	ptCntr = ptCntr + v;
}

void Circle::Out() const{
	std::cout << "\nCircle: center ";
	ptCntr.Out();
	std::cout << ", R = " << R;
}

double Circle::Area() const{
	return M_PI * R * R;
} 