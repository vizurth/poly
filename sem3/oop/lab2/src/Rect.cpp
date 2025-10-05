#include "headers/stdafx.h"
#include "headers/Rect.h"

Rect::Rect() : ptLT_(0.0, 0.0), ptRB_(0.0, 0.0) {}
Rect::Rect(const Rect& other){ // конструктор копирования
	ptLT_ = other.ptLT_;
	ptRB_ = other.ptRB_;
} 
Rect::Rect(double left, double top, double right, double bottom) 
	: ptLT_(left, top), ptRB_(right, bottom) {}

Rect::Rect(const Vector& lt, const Vector& rb){
	ptLT_ = lt;
	ptRB_ = rb;

}

Rect& Rect::operator=(const Rect& other){
	if (this != &other){
		ptLT_ = other.ptLT_;
		ptRB_ = other.ptRB_;
	}
	return *this;
}
	
void Rect::Inflate(double value) {
	ptLT_ = ptLT_ + Vector(-value, value);
	ptRB_ = ptRB_ + Vector(value, -value);
}


void Rect::Inflate(double x_c, double y_c) {
	ptLT_ = ptLT_ + Vector(-x_c, y_c);
	ptRB_ = ptRB_ + Vector(x_c, -y_c);
}


void Rect::Inflate(double left, double top, double right, double bottom) {
	ptLT_ = ptLT_ + Vector(-left, top);
	ptRB_ = ptRB_ + Vector(right, -bottom);
}

void Rect::Out() const {
	std::cout << "\nRect: ";
	ptLT_.Out();
	ptRB_.Out();
}

void Rect::Move(Vector& v){
	ptLT_ = ptLT_ + v;
	ptRB_ = ptRB_ + v;
}

double Rect::Area() const{
	double width = fabs(ptRB_.GetX() - ptLT_.GetX());
	double height = fabs(ptRB_.GetY() - ptLT_.GetY());
	return width * height;
}

Vector Rect::getCenter() const {
    double center_x = (ptLT_.GetX() + ptRB_.GetX()) / 2.0;
    double center_y = (ptLT_.GetY() + ptRB_.GetY()) / 2.0;
    return Vector(center_x, center_y);
}

