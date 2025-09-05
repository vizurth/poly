class Vector
{
private:
	double x, y;	// Координаты вектора на плоскости
public:
	//========== Три конструктора
	Vector (double c1, double c2);
	Vector ();							// Default
	
	//====== Переопределение операций =====//
	Vector& operator= (const Vector& v);	// Присвоение
	void Out();
};
