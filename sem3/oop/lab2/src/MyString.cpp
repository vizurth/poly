#include "headers/stdafx.h"
#include "headers/MyString.h"

#include <cstring>

MyString::MyString() {
  m_pStr = nullptr; // Инициализируем указатель
  Copy("");        // Выделяем память для пустой строки и копируем ее
}

void MyString::Copy (char* s)
{
	delete []m_pStr;
	// Динамически выделяем требуемое количество памяти.
	int len = strlen(s) + 1;
	m_pStr = new char[len];
	// + 1, так как нулевой байт тоже нужно скопировать
	//// Если память выделена, копируем строку-аргумент в строку-член класса
	if (m_pStr){
		strcpy(m_pStr, s);
	}
}

// Определение конструктора.
MyString::MyString (char* s)
{
	m_pStr = 0;
	Copy(s);
}
// Определение деструктора.
MyString::~MyString()
{
	// Освобождение памяти, занятой в конструкторе для строки-члена класса
	delete[] m_pStr;
}

// Метод класса
char* MyString::GetString()
{
	return m_pStr;
}

int MyString::GetLength()
{
	return strlen(m_pStr) + 1;
}


// Операторы

MyString& MyString::operator=(const MyString& other){
	if (this != &other){
		Copy(other.m_pStr);
	}
	return *this;
}
MyString::MyString(const MyString& other) {
    m_pStr = nullptr;
    Copy(other.m_pStr);
}