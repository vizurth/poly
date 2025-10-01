//=======================================================================
//	Лабораторная №3. Шаблоны функций. Шаблоны классов. Стандартные шаблоны С++.
//				Обработка исключений.
//=======================================================================
//Используйте недостающие файлы из лабораторной 2
#include "../../lab2/src/headers/MyVector.h"
#include "../../lab2/src/headers/MyString.h"
#include "headers/stdafx.h"
#include "headers/func.h"
#include "headers/MyStack.h"

#include <iostream>
#include <vector>
#include <string>
#include <list>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::array;
using std::list;


//============= Шаблон функции для вывода с помощью итератора
template <class T> void pr(T& v, string s)
{
	cout<<"\n\n\t"<<s<<"  # Sequence:\n";
	
	// Итератор любого контейнера
	typename T::iterator p;
	int i;

	for (p = v.begin(), i=0;  p != v.end();  p++, i++)
		cout << endl << i+1 <<". "<< *p;
	cout << '\n';
}

template <class T>
void printList(list<T>& lst, string s) {

    cout<<"\n\n\t"<<s<<"  # Sequence:\n";
		
		for (const auto& el : lst) {
        cout << el << " ";
    }

    cout << "\n";
}

void printVector(const Vector& v) {
	v.Out();
}

bool bothGreater2(const Vector& v){
	return v.GetX() > 2.0 && v.GetY() > 2.0;
}

bool bothGreater2ForPtr(Vector* v){
	return v->GetX() > 2 && v->GetY();
}

bool lenGreat5(const Vector& v) {
	return !v > 5;
}

bool beginF(const string& s) {
	return !s.empty() && s[0] == 'f';
}


int main()
{

	//===========================================================
	// Шаблоны функций
	//===========================================================
	// Создайте шаблон функции перестановки двух параметров - Swap().
	// Проверьте работоспособность созданного шаблона с помощью
	// приведенного ниже фрагмента.
	{
		int i = 1, j = -1;
		Swap (i, j);

		double a = 0.5, b = -5.5;
		Swap (a, b);

		Vector u(1,2), w(-3,-4);
		Swap(u, w);

#include "../../lab2/src/headers/MyString.h"
		// Если вы достаточно развили класс MyString в предыдущей работе,
		// то следующий фрагмент тоже должен работать корректно.
		
		MyString s1 ("Your fault"), s2 ("My forgiveness");
		Swap (s1, s2);
		
	}
	//===========================================================
	// Шаблоны классов
	//===========================================================
	// Создайте шаблон класса MyStack для хранения элементов любого типа T.
	// В качестве основы для стека может быть выбран массив.
	// Для задания максимального размера стека может быть использован
	// параметр-константа шаблона
	// Обязательными операциями со стеком являются "Push" и "Pop","GetSize" и "Capacity"
	// Необязательной - может быть выбор по индексу (operator[]).
	// Для того, чтобы гарантировать корректное выполнение этих операций 
	// следует генерировать исключительные ситуации.
	
	// С помощью шаблона MyStack создайте стек переменных типа int, затем
	// стек переменных типа double и, наконец, стек из переменных типа Vector 
	// Если вы подготовите три класса для обработки исключений,
	// то следующий фрагмент должен работать
	try
	{
		cout << "\tTest MyStack\n";
		MyStack <int, 3> stack;

		cout << "\nInteger Stack capacity: " << stack.Capacity();

		stack.Push(1);
		stack.Push(2);
		stack.Push(3);
		
		cout << "\nInteger Stack has: " << stack.GetSize() << " elements";

		//stack.Push(4);	// Здесь должно быть "выброшено" исключение

		cout << "\nInteger Stack pops: " << stack.Pop();
		cout << "\nInteger Stack pops: " << stack.Pop();
		
		cout << "\nInteger Stack has: " << stack.GetSize() << " elements";
		stack.Pop();
		//stack.Pop();		// Здесь должно быть "выброшено" исключение
		stack.Push(2);
		
		int i = stack[3];	// Здесь должно быть "выброшено" исключение
		
		MyStack<Vector, 5> ptStack;
		
		cout << "\nVector Stack capacity: " << ptStack.Capacity();
		
		ptStack.Push(Vector(1,1));
		ptStack.Push(Vector(2,2));
		
		cout << "\nVector Stack pops: ";
		// Используйте метод класса Vector для вывода элемента
		ptStack.Pop().Out();
		
		cout << "\nVector Stack has: " << ptStack.GetSize() << " elements";
	}
	catch (StackOverflow)
	{
		cout << "\nStack overflow";
	}
	catch (StackUnderflow)
	{
		cout << "\nStack underflow";
	}
	catch (StackOutOfRange o)
	{
		o.Out();
	}

	stop;

	////=======================================================================
	//// Контейнеры стандартной библиотеки. Последовательности типа vector
	////=======================================================================
	
	//// Создайте пустой вектор целых чисел. Узнайте его размер с помощью метода size(),
	//// С помощью метода push_back() заполните вектор какими-либо значениями.
	//// Получите новый размер вектора и выведите значения его элементов.
	//// В процессе работы с вектором вы можете кроме количества реально заполненных
	//// элементов (size()) узнать максимально возможное количество элементов (max_size()),
	//// а также зарезервированную память (capacity()).

	vector<int> v;
	v.push_back(-1);
	v.push_back(-2);
	v.push_back(-2);

	size_t n = v.size();
	size_t cap = v.capacity();
	size_t max_n = v.max_size();

	cout << "size: " << n << " capacity: " << cap << endl;
	cout << "max_size: " << max_n << endl;
		
	//// Так как мы часто будем выводить последовательности, то целесообразно
	//// создать шаблон функции для вывода любого контейнера.
	//// Проанализируйте коды такого шабдлона (pr), который приведен выше
	//// Используйте его для вывода вашего вектора

	pr (v, "Vector of ints");
	
	//// Используем другой конструктор для создания вектора вещественных
	//// с начальным размером в 2 элемента и заполнением (222.).
	//// Проверим параметры вектора. Затем изменим размер вектора и его заполнение
	//// (метод - resize()) и вновь проверим параметры.
	
  std::vector<double> vd(2, 222.2);

  pr(vd, "Vector of doubles");

  std::cout << "size: " << vd.size() 
              << ", capacity: " << vd.capacity() 
              << ", max_size: " << vd.max_size() << "\n";

  vd.resize(5, 666.6);

  pr(vd, "After resize");
  std::cout << "size: " << vd.size() 
            << ", capacity: " << vd.capacity() 
            << ", max_size: " << vd.max_size() << "\n";
	
	//// Используя метод at(), а также операцию выбора [], измените значения
	//// некоторых элементов вектора и проверьте результат.
	vd.at(0) = 333.9; 
  vd[3] = 999.9;  
	pr (vd, "After at");
	
	//// Создайте вектор вещественных, который является копией существующего.
	vector<double> wd = vd;
	pr (wd, "Copy");
	
	//// Создайте вектор, который копирует часть существующей последовательности
	vector<double> ud(vd.begin() + 1, vd.begin() + 4);
	pr (ud, "Copy part");

	//// Создайте вектор вещественных, который является копией части обычного массива.
	double ar[] = { 0., 1., 2., 3., 4., 5. };

	vector<double> va(ar, ar + 3);
	pr (va, "Copy part of array");
	
	//// Создайте вектор символов, который является копией части обычной строки
	char s[] = "Array is a succession of chars";
	
	vector<char> vc(s + 5, s + 15);
	pr (vc, "Copy part of c-style string");

	//// Создайте вектор элементов типа Vector и инициализируйте
	//// его вектором с координатами (1,1).
	vector<Vector> vv(7, Vector(1,1));
	
	cout << "\n\nvector of Vectors\n";
	for (int i=0;  i < vv.size();  i++)
		vv[i].Out();

	//// Создайте вектор указателей на Vector и инициализируйте его адресами
	//// объектов класса Vector
	
	vector<Vector*> vp;
	vp.push_back(new Vector(11,11));
	vp.push_back(new Vector(22,22));
	vp.push_back(new Vector(33,33));
	
	cout << "\n\nvector of pointers to Vector\n";
	
	for (size_t i=0;  i < vp.size();  i++)
		vp[i]->Out();

	//// Научитесь пользоваться методом assign и операцией
	//// присваивания = для контейнеров типа vector.
	vp.assign(7, new Vector(77, 77)); 

	cout << "\n\nAfter assign\n";
	for (size_t i=0;  i < vp.size();  i++)
		vp[i]->Out();
	
	//// Декларируйте новый вектор указателей на Vector и инициализируйте его 
	//// с помощью второй версии assign
	vector<Vector*> vpNew;
	vpNew.assign(vp.begin(), vp.end());
	
	cout << "\n\nNew vector after assign\n";
	for (size_t i=0;  i < vpNew.size();  i++)
		vpNew[i]->Out();


	//// На базе шаблона vector создание двухмерный массив и
	//// заполните его значениями разными способами.
	//// Первый вариант - прямоугольная матрица
	//// Второй вариант - ступенчатая матрица

	const double val = 3.33;
	const int rows = 3, cols = 2;
	vector<vector<double>> vdd(rows, vector<double>(cols, val));


	std::cout << "\n\n\tRectangular matrix:\n";
  for (size_t i = 0; i < vdd.size(); i++) {      // строки
        for (size_t j = 0; j < vdd[i].size(); j++) // столбцы
            std::cout << vdd[i][j] << "\t";       // табуляция для выравнивания
        std::cout << "\n";
  }
	
	////========= Ступенчатая матрица
	for (size_t i=0;  i < vdd.size();  i++)
		vdd[i] = vector<double>(i+1, double(i));
	
	cout << "\n\n\tTest vector of vector<double>\n";
	for (size_t i=0;  i < vdd.size();  i++)
	{
		cout << endl;
		for (size_t j=0;  j < vdd[i].size();  j++)
			cout << vdd[i][j] << " ";
	}	
	cout << endl;

	//===================================
	// Простейшие действия с контейнерами
	//===================================
	//3б. Получение значения первого и последнего элементов последовательности.
	//Получение размера последовательности. Присваивание значений
	//элементов одной последовательности элементам другой - assign().

	//Создайте и проинициализируйте вектор из элементов char. Размер -
	//по желанию.

	vector<char> charVec = {'H', 'E', 'L', 'L', 'O'};


	//Создайте и проинициализируйте массив из элементов char. Размер -
	//по желанию.
	const size_t sz = 5;
	array<char, sz> cMas;
	for (size_t i = 0; i < sz; ++i) {
		cMas[i] = static_cast<char>('c' + i);
	}

	//Получите значение первого элемента вектора ( front() )

	cout << charVec.front() << endl;


	//Получите значение последнего элемента вектора ( back() )

	cout << charVec.back() << endl;

	//Получите размер вектора

	cout << charVec.size() << endl;


	//Присвойте вектору любой диапазон из значений массива cMas.

	cout << "старый размер: " << charVec.size() << endl;

	charVec.assign(cMas.begin() + 1, cMas.begin() + 3);

	//Проверьте размер вектора, первый и последний элементы.

	cout << "новый размер: " << charVec.size() << endl;
  cout << "новый первый: " << charVec.front() << endl;
  cout << "новый послед: " << charVec.back() << endl;
  
	stop;


	//3в. Доступ к произвольным элементам вектора с проверкой - at()
	//и без проверки - []
	//Создайте неинициализированный вектор из 8 элементов char - vChar2.
	//С помощью at() присвойте четным элементам вектора значения
	//элементов vChar1 из предыдущего задания,
	//а с помощью [] присвойте нечетным элементам вектора vChar2 значения
	//массива {'K','U','K','U'}.

	vector<char> vChar1 = {'A', 'B', 'C', 'D', 'E', 'F'};
  vector<char> vChar2(8);
  array<char, 4> charArray = {'K', 'U', 'K', 'U'};

	for (size_t i = 0; i < vChar1.size(); ++i) {
    if (i < 4) { 
      vChar2.at(i * 2) = vChar1.at(i);
    }
  }

	for (size_t i = 0; i < charArray.size(); ++i) {
    vChar2[i * 2 + 1] = charArray[i];
  }
	
	  stop;
	//Попробуйте "выйти" за границы вектора с помощью at() и
	//с помощью []. Обратите внимание: что происходит при
	//попытке обращения к несуществующему элементу в обоих случаях
	try {
    vChar2.at(66) = 'e'; 
  } catch (const std::out_of_range& e) {
    std::cerr << "except catched: " << e.what() << std::endl;
  }
	char c = vChar2[66]; // мусор
	// [] -> *(arr + 66)
  
	stop;

	//3г.Добавьте в конец вектора vChar2  - букву Z (push_back()). Для
	//расширения кругозора можете ее сразу же и выкинуть (pop_back())
	
	vChar2.push_back('Z');
	vChar2.pop_back();
  
	stop;

	//3д. Вставка-удаление элемента последовательности insert() - erase()
	//Очистка последовательности - clear()

	//Вставьте перед каждым элементом вектора vChar2 букву 'W'

for (int i = vChar2.size() - 1; i >= 0; --i)
    vChar2.insert(vChar2.begin() + i, 'W');
	
	

	//Вставьте перед 5-ым элементом вектора vChar2 3 буквы 'X'

	vChar2.insert(vChar2.begin() + 4, 3, 'X');
	pr(vChar2, "vChar2");


	//Вставьте перед 2-ым элементом вектора vChar2 с третьего по
	//шестой элементы массива "aaabbbccc"

	vector<char> letters = {'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c'};
	vChar2.insert(vChar2.begin() + 1, letters.begin() + 2, letters.begin() + 6);

	pr(letters, "letters");
  
	//Сотрите c первого по десятый элементы vChar2
	if (vChar2.size() >= 10) 
        vChar2.erase(vChar2.begin(), vChar2.begin() + 10);
	
	pr(vChar2, "vChar2 erace");

  
	stop;

	//Уничтожьте все элементы последовательности - clear()
	vChar2.clear();
	pr(vChar2, "vChar2 clear");


	stop;

	//Создание двухмерного массива
	vector<vector<int>> mtx(3, vector<int>(6, 3));

	stop;

///////////////////////////////////////////////////////////////////

	////Задание 4. Списки. Операции, характерные для списков.
	////Создайте два пустых списка из элементов Vector - ptList1 и
	////ptList2

	list<Vector> ptList1;
	list<Vector> ptList2;
	

	//Наполните оба списка значениями с помощью методов push_back(),
	//push_front, insrert()

	ptList1.push_back(Vector(3.3, 4.4));
  ptList1.push_front(Vector(1.1, 2.2));
  ptList1.push_back(Vector(5.5, 6.6));

  ptList1.insert(ptList1.begin(), Vector(0.0, 0.0));

	ptList2.push_back(Vector(2.0, 2.0));
  ptList2.push_front(Vector(6.0, 8.0));
  ptList2.push_back(Vector(4.0, 3.0));


	//Отсортируйте списки - sort().
	//Подсказка: для того, чтобы работала сортировка, в классе Vector
	//должен быть переопределен оператор "<"

	printList(ptList1, "ptList1 before sort");
  printList(ptList2, "ptList2 before sort");

  //ptList1.sort();
  //ptList2.sort();

  printList(ptList1, "ptList1 after sort");
  printList(ptList2, "ptList2 after sort");
  
	stop;

	//Объедините отсортированные списки - merge(). Посмотрите: что
	//при этом происходит со вторым списком.

	ptList1.merge(ptList2);
  printList(ptList1, "ptList1 after merge");
  printList(ptList2, "ptList2 after merge (now empty)");


	stop;

	//Исключение элемента из списка - remove()
	//Исключите из списка элемент с определенным значением.
	//Подсказка: для этого необходимо также переопределить
	//в классе Vector оператор "=="

	ptList1.remove(Vector(1.1, 2.2));
  printList(ptList1, "ptList1 after removing (1.1, 2.2)");

	stop;

///////////////////////////////////////////////////////////////////

	//Задание 5. Стандартные алгоритмы.Подключите заголовочный файл
	// <algorithm>
	//5а. Выведите на экран элементы ptList1 из предыдущего
	//задания с помощью алгоритма for_each()


	std::cout << "\nptList1 (for_each): ";
	std::for_each(ptList1.begin(), ptList1.end(), printVector);
	std::cout << std::endl;


	stop;

	//5б.С помощью алгоритма find() найдите итератор на элемент Vector с
	//определенным значением. С помощью алгоритма find_if() найдите
	//итератор на элемент, удовлетворяющий определенному условию, 
	//например, обе координаты точки должны быть больше 2.
	//Подсказка: напишите функцию-предикат, которая проверяет условие
	//и возвращает boolean-значение (предикат может быть как глобальной
	//функцией, так и методом класса)


	auto it = std::find(ptList1.begin(), ptList1.end(), Vector(5.5, 6.6));
	if (it != ptList1.end()) {
		std::cout << "\nНайден: " << *it << std::endl;
	} else {
		std::cout << "\nНе найден (1.1, 2.2)" << std::endl;
	}

	// find_if по условию
	auto it2 = std::find_if(ptList1.begin(), ptList1.end(), bothGreater2);
	if (it2 != ptList1.end()) {
		cout << "Первый >2: " << *it2 << endl;
	}

	stop;

	//Создайте список из указателей на элеметы Vector. С помощью 
	//алгоритма find_if() и предиката (можно использовать предикат - 
	//метод класса Vector, определенный в предыдущем задании) найдите в
	//последовательности элемент, удовлетворяющий условию

	list<Vector*> ptrList;
	for (auto& v: ptList1) {
		ptrList.push_back(&v);
	}

	auto it3 = std::find_if(ptrList.begin(), ptrList.end(), bothGreater2ForPtr);
	if (it3 != ptrList.end()) {
		cout << "Найден в ptrList: " << **it3 << endl;
	}

	
	  stop;

	//5в. Создайте список элементов Vector. Наполните список
	//значениями. С помощью алгоритма replace() замените элемент
	//с определенным значением новым значением. С помощью алгоритма
	//replace_if() замените элемент, удовлетворяющий какому-либо
	//условию на определенное значение. Подсказка: условие
	//задается предикатом.

	list<Vector> lst;

	lst.push_back(Vector(1,1));
	lst.push_back(Vector(2,2));
	lst.push_back(Vector(3,3));

	printList(lst, "До replace");

	std::replace(lst.begin(), lst.end(), Vector(2,2), Vector(9,9));

	printList(lst, "После replace");

	std::replace_if(lst.begin(), lst.end(), lenGreat5, Vector(0,0));

	printList(lst, "После replace_if");


  
	stop;


	//5г. Создайте вектор строк (string). С помощью алгоритма count()
	//сосчитайте количество одинаковых строк. С помощью алгоритма
	//count_if() сосчитайте количество строк, начинающихся с заданной
	//буквы

	vector<string> words = {"one", "one", "one", "two", "three", "three", "four", "five", "six", "seven"};

	int c1 = std::count(words.begin(), words.end(), "one");

	cout << "\n\nCount of 'one': " << c1 << endl;

	int c2 = std::count_if(words.begin(), words.end(), beginF);
	std::cout << "\nСколько начинаются с 'f': " << c2 << std::endl;

	//5д. С помощью алгоритма count_if() сосчитайте количество строк,
	//которые совпадают с заданной строкой. Подсказка: смотри тему
	//объекты-функции


	string target = "three";

	int c3 = std::count_if(words.begin(), words.end(), [&target](const string& s) {return s == target;});

	std::cout << "\nСколько равно 'three': " << c3 << std::endl;

	stop;


	cout <<"\n\n";
}