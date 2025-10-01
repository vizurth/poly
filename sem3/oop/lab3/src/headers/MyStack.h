#pragma once
#include <exception>
#include <iostream>

template<class T, int N>
class MyStack{
	private:
		T arr[N];
		int size;
	public:
		MyStack();
		void Push(const T& elem);
		T Pop();
		int GetSize();
		int Capacity();

		T& operator[](int index);
		const T operator[](int index) const;
};


class StackOverflow : public std::runtime_error {
public:
    StackOverflow() : std::runtime_error("\nError! Stack is full!") {}
};

class StackUnderflow : public std::runtime_error {
public:
    StackUnderflow() : std::runtime_error("\nError! Stack is empty!") {}
};

class StackOutOfRange {
public:
    int index;
    StackOutOfRange(int i) : index(i) {}
    void Out() const {
        std::cout << "\nError! Index " << index << " is out of range!\n";
    }
};

#include "../tpl/MyStack.tpp"