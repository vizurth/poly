#pragma once

#include <iostream>


class Point {
private:
	int x;
	int y;

public:
	Point(int x_val, int y_val) : x(x_val), y(y_val) {}

	void setX(int x){
		this->x = x;
	}

	void setY(int y){
		this->y = y;
	}

	int getX() const{
		return x;
	}

	int getY() const{
		return y;
	}

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  }

  bool operator<(const Point& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class PointModifier {
private:
    int x;
    int y;

public:
    PointModifier(int x, int y) : x(x), y(y) {}

    void operator()(Point& p) const { 
        p.setX(p.getX() + x);
        p.setY(p.getY() + y);
    }
};