class Vector
{
private:
	double x, y;	// Координаты вектора на плоскости
public:
	//========== Три конструктора
	Vector (double c1, double c2);
	Vector ();							// Default
	Vector(const Vector& other);
	
	//====== Переопределение операций =====//
	Vector& operator= (const Vector& other);	// Присвоение
	Vector operator+ (const Vector& other);

	Vector operator* (double scalar) const;
	friend Vector operator*(double scalar, const Vector& vec);
	friend double operator*(const Vector& a, const Vector& b);

	double operator!() const;
	bool operator>(const Vector& other) const;
	bool operator==(const Vector& other) const;
	void Out();
};