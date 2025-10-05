#pragma once

class MyString
{
private:
	char* m_pStr;		// Элемент данных класса (адрес строки)
public:
  MyString ();
	MyString (const char* s);	// Объявление конструктора
	MyString(const MyString& other); // конструктор копирования
  ~MyString();		// Объявление деструктора
	

	void Copy (const char* s);
	char* GetString();	// Объявление метода (accessor)
	int GetLength();	// Объявление метода (длина строки)

	friend std::ostream& operator<<(std::ostream& os, const MyString& str);
	// операторы
	MyString& operator= (const MyString& other); // конструктор присваивания	
};
