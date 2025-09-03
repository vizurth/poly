#include "stdafx.h"
#include "MyVector.h"


Vector::Vector (double c1, double c2)
{
	x=c1;   y=c2;
}

Vector::Vector ()
{
	x = y = 0.;
}

void Vector::Out()
{
	cout << "\nVector:  x = " << x << ",  y = " << y;
}

//====== Переопределение операций =====//
Vector& Vector::operator= (const Vector& v)	// Присвоение
{
	if (this == &v)
		return *this;
	x = v.x;
	y = v.y;
	return *this;
}
