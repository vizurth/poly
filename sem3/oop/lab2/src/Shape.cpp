#include "headers/stdafx.h"
#include "headers/Shape.h"

int Shape::Count = 0;
Shape* Shape::head_ = nullptr;

Shape::Shape() {
	// добавление в связный список
	this->next_ptr = head_;
	head_ = this;
	Count++;
}

Shape::~Shape() { // удаление за O(n)
	// удаление из связного списка
	Shape* prev = nullptr;
	Shape* cur = head_;
	while (cur != nullptr) {
		if (cur == this) {
			if (prev == nullptr) {
				head_ = cur->GetNext();
			} else {
				prev->SetNext(cur->GetNext());
			}
			break;
		}
		prev = cur;
		cur = cur->GetNext();
	}
	Count--;
}

// вывод количества обьектов интерфейса
void Shape::PrintCount() {
	std::cout << "\n Now there are " << Count << " shapes";
}

// геттеры

int Shape::GetCount() { return Count; }

Shape* Shape::GetHead() { return head_; }