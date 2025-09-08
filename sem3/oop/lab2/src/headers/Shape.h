#pragma once
class Vector; // forward declaration per requirement

class Shape {
private:
	static int Count;
	static Shape* head_;
	Shape* next_ptr;

public:
	Shape();
	virtual ~Shape();

	virtual void Move (Vector& v) = 0;
	virtual void Out() const = 0;
	virtual double Area() const = 0;

	static void PrintCount();
	static int GetCount();
	static Shape* GetHead();

	Shape* GetNext() const { return next_ptr; }
	void SetNext(Shape* n) { next_ptr = n; }
};