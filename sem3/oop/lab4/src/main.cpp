// Контейнеры STL: 
//deque, stack, queue, priority_queue
//set, multiset, map, multimap
//Итераторы. Стандартные алгоритмы. Предикаты.

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <deque>

#include <cctype>

#include "../../lab2/src/headers/MyString.h"
#include "../../lab2/src/headers/Rect.h"
#include "headers/Point.h"
#include "headers/func.h"


using namespace std;	
#define	stop

bool Pred1_1(const Point& p) {
  return (p.getX() == 7) && (p.getY() == 7);
}

void to_lower_case(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

int main()
{

	//Очередь с двумя концами - контейнер deque

	//Создайте пустой deque с элементами типа Point. С помощью
	//assign заполните deque копиями элементов вектора. С помощью
	//разработанного Вами в предыдущем задании универсального шаблона
	//выведите значения элементов на печать

	vector<Point> pntV = {{1,7}, {3, 2}, {3, 6}};
	deque<Point> pntD;

	pntD.assign(pntV.begin(), pntV.end());
	printContainer(pntD, "deque point:");

	//Создайте deque с элементами типа MyString. Заполните его значениями
	//с помощью push_back(), push_front(), insert()
	//С помощью erase удалите из deque все элементы , в которых строчки
	//начинаются с 'A' или 'a'

	deque<MyString> myStrings;

	myStrings.push_back(MyString("aaa"));
	myStrings.emplace_back("alsjd");
	myStrings.push_front(MyString("prog"));
	myStrings.emplace_front("Golang");
	myStrings.insert(myStrings.begin() + 2, MyString("C++"));

	printContainer(myStrings, "deque MyString:");

	for (auto it = myStrings.begin(); it != myStrings.end();){
		char firstChar = it->GetString()[0];
		if (firstChar == 'A' || firstChar == 'a') { 
			it = myStrings.erase(it); // erase вернет следующий итератор
		} else {
			++it;
		}
	}

	printContainer(myStrings, "deque MyString after erase:");


	////////////////////////////////////////////////////////////////////////////////////


	//Напишите шаблон функции для вывода значений stack, queue, priority_queue
	//Подумайте, как "получать" данное "с верхушки"?
	//Что происходит с контейнерами после вывода значений?

  // vector
  vector<int> v = {5, 3, 8, 1};
  printContainer(v, "Vector:");

  // stack
  stack<int> st;
  for (auto x : v) st.push(x);
  printAdapter(st, "Stack:");

  // queue
  queue<int> q;
  for (auto x : v) q.push(x);
  printAdapter(q, "Queue:");

  // priority_queue
  priority_queue<int> pq;
  for (auto x : v) pq.push(x);
  printAdapter(pq, "Priority Queue:");



	////////////////////////////////////////////////////////////////////////////////////
	//stack

	//Создайте стек таким образом, чтобы
	//а) элементы стека стали копиями элементов вектора
	//б) при выводе значений как вектора, так и стека порядок значений был одинаковым 



	//Сравнение и копирование стеков
	//а) создайте стек и любым способом задайте значения элементов
	//б) создайте новый стек таким образом, чтобы он стал копией первого
	//в) сравните стеки на равенство
	//г) модифицируйте любой из стеком любым образом (push, pop, top)
	//д) проверьте, какой из стеков больше (подумайте, какой смысл вкладывается в такое сравнение)
	{
		stack<int> stA;
		stA.push(1);
		stA.push(2);
		stA.push(3);

		printAdapter(stA, "stack A");

		stack<int> stB = stA;
		printAdapter(stA, "stack B");


		if (stA == stB) {
			cout << "\nstacks same" << endl;
		} else {
			cout << "\nstacks not same" << endl;
		}

		stA.push(5);

		printAdapter(stA, "stack A after push 5");

		if (stA > stB) cout << 1 << endl;
		else cout << 0 << endl;

		if (stA.size() > stB.size()) cout << 1 << endl;
		else cout << 0 << endl;

	stop

	}


	////////////////////////////////////////////////////////////////////////////////////
	/*
	template<
		class T,
		class Container = std::deque<T>
	> class queue;
	*/

	//queue

	//Создайте очередь, которая содержит указатели на объекты типа Point,
	//при этом явно задайте базовый контейнер.
	//Измените значения первого и последнего элементов посредством front() и back()
	//Подумайте, что требуется сделать при уничтожении такой очереди?
	{
		deque<Point*> dq;
		dq.push_back(new Point(0,0));
		dq.push_back(new Point(1,1));
		dq.push_back(new Point(2,2));
		dq.push_back(new Point(3,3));

		queue<Point*, deque<Point*>> q(dq);

		printAdapter(q, "queue before edit");
		
		q.front()->setX(10);
		q.front()->setY(20);

		q.back()->setX(40);
		q.back()->setY(50);

		printAdapter(q, "queue after edit");

		//очистить память
		while (!q.empty()){
			delete q.front();
			q.pop();
		}

	}
	////////////////////////////////////////////////////////////////////////////////////

	/*
	template<
		class T,
		class Container = std::vector<T>,
		class Compare = std::less<typename Container::value_type>
	> class priority_queue;
	*/

	//priority_queue
	//а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
	//б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
	//в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
	//		что сравнивается при вставке?


	{
		const char* lits[] = {"ccc", "aaa", "bbb"};
		// неверно т.к const char* (в случае с int || double было бы все ок)
		// в случае строк < сравнивает адреса в памяти а не строки
		priority_queue<const char*>pq1(lits, lits + 3);

		printAdapter(pq1, "pq");

		auto cmp = [](const char* a,const char* b){
			return strcmp(a, b) < 0; // true : if(a < b)
		};

		priority_queue<const char*, vector<const char*>, decltype(cmp)> pq2(
			lits, lits + 3, cmp
		);
		cout << "!!!!" << endl;

		printAdapter(pq2, "pq2");
	}
	
	
	////////////////////////////////////////////////////////////////////////////////////
	//set
	//a) создайте множество с элементами типа Point - подумайте, что необходимо определить
	//		в классе Point (и каким образом)
	//б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
	//в) попробуйте изменить любое значение...
	//г) Создайте два множества, которые будут содержать одинаковые значения
	//		типа int, но занесенные в разном порядке
	//д) Вставьте в любое множество диапазон элементов из любого другого
	//	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?)

	set<Point> pointSet;

	pointSet.insert(Point(1,1));
	pointSet.insert(Point(2,2));
	pointSet.insert(Point(3,3));
	pointSet.insert(Point(4,4));

	printContainer(pointSet, "set");
	// нельзя изменить можно только сделать erase и insert


	set<int> setA = {5, 1, 8, 3};
	set<int> setB = {3, 8, 1, 5};

	std::set<int> mySet;
	std::vector<int> sourceVector = {10, 20, 10, 5, 30, 20};

	mySet.insert(sourceVector.begin(), sourceVector.end()); 

	printContainer(mySet, "mySet");


	////////////////////////////////////////////////////////////////////////////////////
	/*
		template<
			class Key,
			class Compare = std::less<Key>,
			class Allocator = std::allocator<Key>
		> class multiset;
		*/
	{
		multiset<int> nums = {10, 5, 20, 10, 30, 5};
		nums.insert(10);
		
		printContainer(nums, "multiset nums");
	}


	////////////////////////////////////////////////////////////////////////////////////
	//map	
	//а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
	//	при этом строки задаются строковыми литералами
	//б) заполните контейнер значениями посредством operator[] и insert()
	//в) распечатайте содержимое

	//е) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")

	{
		map<const char*, int> mp;

		mp["Иванова"] = 1234;
		mp.insert(make_pair("Horwad", 120394));
		mp.insert({"ee", 20834});

		int temp = mp["Иванова"];
		mp["Петрова"] = temp;
		mp.erase("Иванова");

		printMap(mp);
	}


	////////////////////////////////////////////////////////////////////////////////////
	//multimap
	//а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
	//		несколько русских значений - pair<string,string>, например: strange: чужой, странный...
	//б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью 
	//		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)
	//в) Выведите все содержимое словаря на экран
	//г) Выведите на экран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
	//		итераторов можно использовать методы lower_bound() и upper_bound()
	{
		multimap<string, string> dictionary = {
			{"strange", "чужой"},
			{"book", "книга"},
			{"strange", "странный"}, // Дублируем ключ "strange"
			{"book", "бронировать"},  // Дублируем ключ "book"
			{"test", "тест"},
			{"test", "испытание"}
		};

		dictionary.insert({"book", "книга"});
    dictionary.insert(make_pair("book", "бронировать"));

		for (const auto& pair : dictionary) cout << pair.first << " -> " << pair.second << endl;

		string key = "strange";
    auto range = dictionary.equal_range(key); // pair<it, it>  замена lower ^ upper_bound

		cout << "\nПеревод для strange:\n";
    for (auto it = range.first; it != range.second; ++it) {
        cout << "  -> " << it->second << endl;
    }
		
	}

///////////////////////////////////////////////////////////////////

	//Итераторы

	//Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
	//нужно перегрузить в классе Point. Создайте вектор, элементы которого 
	//являются копиями элементов set, но упорядочены по убыванию

	// перегружаем operator==
	set<Point> p_set = {{1, 7}, {3, 2}, {3, 6}, {0, 0}};
  vector<Point> p_vec(p_set.rbegin(), p_set.rend());

	printContainer(p_vec, "p_vec"); 


	//Потоковые итераторы. С помощью ostream_iterator выведите содержимое
	//vector и set из предыдущего задания на экран.

	ostream_iterator<Point> out(cout, " ");

  copy(p_vec.begin(), p_vec.end(), out);
	cout << endl;
  copy(p_set.begin(), p_set.end(), out);
	cout << endl;



	//Итераторы вставки. С помощью возвращаемых функциями:

	vector<int> src = {1, 2, 3};
  deque<int> d;
  list<int> l;
	vector<int> ve;

	/*
		template< class InputIt, class OutputIt >
		OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
	*/

	//back_inserter() - deque, vector, list
	copy(src.begin(), src.end(), back_inserter(d));
	//front_inserter() - deque, list
	copy(src.begin(), src.end(), front_inserter(l));
	//inserter() - w all
	copy(src.begin(), src.end(), inserter(ve, ve.begin()));
	//итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
	//какие из итераторов вставки можно использовать с каждым контейнером.



///////////////////////////////////////////////////////////////////

	//Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

	// алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
	//(массив, vector, list...)
	//С помощью алгоритма for_each в любой последовательности с элементами любого типа
	//распечатайте значения элементов
	//Подсказка : неплохо вызываемую функцию определить как шаблон

	vector<int> nums = {10, 20, 30};
  list<string> words = {"hello", "world"};
    
  for_each(nums.begin(), nums.end(), print<int>);
  cout << endl;

  for_each(words.begin(), words.end(), print<string>);
  cout << endl;
	stop

	//С помощью алгоритма for_each в любой последовательности с элементами типа Point
	//измените "координаты" на указанное значение (такой предикат тоже стоит реализовать 
	//как шаблон) и выведите результат с помощью предыдущего предиката

	vector<Point> points = {{1, 1}, {2, 2}, {3, 2}, {1, 7}, {1, 7}};

	// PointTransformer класс функтор (повторяет поведение функций)
  for_each(points.begin(), points.end(), PointModifier(5, 5));

  for_each(points.begin(), points.end(), print<Point>);
  cout << "\n";


	//С помощью алгоритма find() найдите в любой последовательности элементов Point
	//все итераторы на элемент Point с указанным значением.

	Point target_point = {6, 12};

  auto it = points.begin();
  int count = 0;
	cout << "\nсписок совпадений:\n";
	while (true) {
    it = find(it, points.end(), target_point);
    if (it != points.end()) {
      cout << distance(points.begin(), it) << endl;
      ++it; 
      count++;
    } else {
      break;
    }
  }

  cout << "\nколичество: " << count << endl;

	
	
	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Point. 
	/*
		template< class RandomIt >
		void sort( RandomIt first, RandomIt last );
	*/
	////По умолчанию алгоритм сортирует последовательность по возрастанию.
	//Что должно быть определено в классе Point?
	// Замечание: обобщенный алгоритм sort не работает со списком, так как
	//это было бы не эффективно => для списка сортировка реализована методом класса!!!
	
	printContainer(points, "Points before sort:");

	sort(points.begin(), points.end()); // сортирует по первому элементу

	printContainer(points, "Points before sort:");


	//Создайте глобальную функцию вида: bool Pred1_1(const Point& ), которая будет вызываться
	//алгоритмом find_if(), передавая в качестве параметра очередной элемент последовательности.
	//С помощью алгоритма find_if() найдите в любой последовательности элементов Point
	//итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
	//[-n, +m].

	auto it1 = find_if(points.begin(), points.end(), Pred1_1);

	if (it1 != points.end()) {
		cout << distance(points.begin(), it1) << endl;
		cout << "{" << it->getX() << ", " << it1->getY() << "}" << endl;
	}




	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
	//располагая прямоугольники по удалению центра от начала координат.
	
	vector<Rect> rects = {
			Rect(100, 100, 101, 101),
			Rect(0, 0, 10, 10),
			Rect(10, 0, 15, 5)
		};

	printContainer(rects, "rects before sort");

	sort(rects.begin(), rects.end(), [](const Rect& a, const Rect& b) {
		Vector center_a = a.getCenter();
		Vector center_b = b.getCenter();
			
		double dst_a = center_a.GetX() * center_a.GetX() + center_a.GetY() * center_a.GetY();
		double dst_b = center_b.GetX() * center_b.GetX() + center_b.GetY() * center_b.GetY();

		return dst_a < dst_b;
	});

	printContainer(rects, "rects after sort");


	{//transform
		/*
			template< class InputIt, class OutputIt, class UnaryOp >
			OutputIt transform( InputIt first1, InputIt last1, OutputIt d_first, UnaryOp unary_op );
		*/
		//Напишите функцию, которая с помощью алгоритма transform переводит 
		//содержимое объекта string в нижний регистр.
		//Подсказка: класс string - это "почти" контейнер, поэтому для него
		// определены методы begin() и end()

		string my_string = "Linear Algebra and Math Structures";
		to_lower_case(my_string);
		cout << my_string << "\n\n";


		//Заполните list объектами string. С помощью алгоритма transform сформируте
		//значения "пустого" set, конвертируя строки в нижний регистр
		/*
			template< class Container >
			std::insert_iterator<Container>
			inserter( Container& c, typename Container::iterator i );
		*/

		list<string> words = {"ALGEBRA", "Math", "go", "algebra", "go"};
		set<string> lower;
		// inserter из-за пустого set'a
		transform(words.begin(), words.end(),
					inserter(lower, lower.begin()),
					[](const string& s) {
						string temp = s;
						to_lower_case(temp); 
						return temp;
					});
		
		for (const auto& word : lower) 
        	cout << word << " ";
		cout << "\n\n";

		stop
	}
	{// map
		
		//Сформируйте любым способом вектор с элементами типа string.
		//Создайте (и распечатайте для проверки) map<string, int>, который будет
		//содержать упорядоченные по алфавиту строки и
		//количество повторений каждой строки в векторе
	



		vector<string> words = {"ALGEBRA", "Math", "c++", "ALGEBRA", "C++"};
		map<string, int> word_cnt;

		for (const auto& word : words) {
			word_cnt[word]++;
		}
		
		printMap(word_cnt);

	}

	


	return 0;
}

