// // Контейнеры STL: 
// //deque, stack, queue, priority_queue
// //set, multiset, map, multimap
// //Итераторы. Стандартные алгоритмы. Предикаты.

// #include <iostream>
// #include <vector>
// #include <list>
// #include <set>
// #include <map>
// #include <deque>
// #include <algorithm>
// #include "point.h"
// #include "MyString.h"
// #include "Rect.h"
// #include "Shape.h"
// #include "MyVector.h"
// #include <stack>
// #include <queue>
// #include <iterator>
// #include <cmath>
// #include <cstring>
// using namespace std;	
// #define	  stop //__asm nop
// template <class T> void pr(T& v, string s)
// {
// 	cout<<"\n\n\t"<<s<<"  # Sequence:\n";
	
// 	// Итератор любого контейнера
// 	typename T::iterator p;
// 	int i;

// 	for (p = v.begin(), i=0;  p != v.end();  p++, i++)
// 		cout << endl << i+1 <<". "<< *p;
// 	cout << '\n';
// }
// template <typename T>
// void printSQPQ(T  sqpq){
// 		while(!sqpq.empty()){
// 			 if constexpr (std::is_same_v<T, std::queue<typename T::value_type>>) {
// 				std::cout << sqpq.front() << " ";
// 			 }else{
// 				cout<< sqpq.top()<<" "; 
// 			 }
// 			sqpq.pop();
// 		}
// 		cout<<"\n";
// 	}

// template <typename T>
// void print(const T& el){
// 	cout<< el << "; ";
// }

// template <typename T>
// void modifyPoint(Point& point, T delta_x, T delta_y){
//     point.set_x(point.get_x() + delta_x);
//     point.set_y(point.get_y() + delta_y);
// }
// template <int n, int m>
// bool Pred1_1(const Point& point){
// 	return (point.get_x() >= n && point.get_x()<=m) && (point.get_y() >= n && point.get_y()<=m); 
// }


// double distanceFromOrigin(const Rect& rect){
// 	Vector center = rect.getCenterCoord();
//     return sqrt(center.get_x()*center.get_x()+center.get_y()*center.get_y());
// }

// bool compareByDistance(const Rect& a, const Rect& b) {
//     return distanceFromOrigin(a) < distanceFromOrigin(b);
// }

// void tol(string& str){
// 	transform(str.begin(), str.end(), str.begin(), [](unsigned char s){return tolower(s);});
// }

// int main()
// {

// 	//Очередь с двумя концами - контейнер deque
	
// 	//массив указателей на массивы
	
// 	// Обеспечивать произвольный доступ к элементам (как
// 	// vector ⇒ at(), operator[]) O(1)
// 	// Эффективная работа с началом последовательности (как у
// 	// list ⇒ push_front(), pop_front()) и с концом O(1)
// 	// вставка или удаление не из начала или не из конца O(n)

// 	//Создайте пустой deque с элементами типа Point. С помощью
// 	//assign заполните deque копиями элементов вектора. С помощью
// 	//разработанного Вами в предыдущем задании универсального шаблона
// 	//выведите значения элементов на печать
	
// 	deque<Point> pointDeque;
// 	pointDeque.assign(3, Point(5,2));
// 	//pointDeque.assign(3, Point(4,2));

// 	pr(pointDeque, "Deque of points");

// 	//Создайте deque с элементами типа MyString. Заполните его значениями
// 	//с помощью push_back(), push_front(), insert()
// 	//С помощью erase удалите из deque все элементы, в которых строчки
// 	//начинаются с 'A' или 'a'
	
// 	deque<MyString> myStrDeque;
// 	myStrDeque.push_back(MyString("ivanov"));
// 	myStrDeque.push_front(MyString("senya"));
// 	myStrDeque.insert(myStrDeque.begin() + 2, MyString("19"));
// 	myStrDeque.push_front(MyString("apple"));
	
// 	struct StartsWithA {
//     bool operator()(const MyString& str) const {
// 			const char* s = str.GetString();
// 			if (s && s[0] != '\0') {
// 				char firstChar = std::tolower(static_cast<unsigned char>(s[0]));
// 				return firstChar == 'a';
// 			}
// 			return false;
// 		}
// 	};
// 	myStrDeque.erase(
//     std::remove_if(myStrDeque.begin(), myStrDeque.end(), StartsWithA()),
// 		myStrDeque.end()
// 	);
// 	////////////////////////////////////////////////////////////////////////////////////


// 	//Напишите шаблон функции для вывода значений stack, queue, priority_queue
// 	//Подумайте, как "получать" данное "с верхушки"?
// 	//Что происходит с контейнерами после вывода значений?

// 	/*template <typename T>
// 	void printSQPQ(T  sqpq){
// 		while(!sqpq.empty()){
// 			 if constexpr (std::is_same_v<T, std::queue<typename T::value_type>>) {
// 				std::cout << container.front() << " ";
// 			 }else{
// 				cout<< sqpq.top()<<" "; 
// 			 }
// 			sqpq.pop();
// 		}
// 		cout<<"\n";
// 	}*/


// 	////////////////////////////////////////////////////////////////////////////////////
// 	//stack
	
// 	// адаптер deque. 
//     // помещать и доставать можно только элементы с конца.
// 	// O(1) 	
// 	//из базового контейнера недоступны методы; итераторы; псевдонимы
	
// 	//Создайте стек таким образом, чтобы
// 	//а) элементы стека стали копиями элементов вектора
// 	//б) при выводе значений как вектора, так и стека порядок значений был одинаковым 
// 	vector<int> vec= {5,2,4,2};
// 	stack<int> stck;
// 	 for (int i = vec.size() - 1; i >= 0; i--) {
//         stck.push(vec[i]);
//     }
// 	pr(vec, "vector");
// 	cout<<"\n\n Stack\n";
// 	printSQPQ(stck);


// 	//Сравнение и копирование стеков
// 	//а) создайте стек и любым способом задайте значения элементов
// 	//б) создайте новый стек таким образом, чтобы он стал копией первого
// 	//в) сравните стеки на равенство
// 	//г) модифицируйте любой из стеком любым образом (push, pop, top)
// 	//д) проверьте, какой из стеков больше (подумайте, какой смысл вкладывается в такое сравнение)
	
// 	//сравнение лексиграфически, но если элементы равны, то больше тот в котором больше элементов (наш случай)
// 	{
// 		stack<int> stck;
// 		for (int i = 0; i <10; i++) {
// 			stck.push(i);
// 		}
// 		stack<int> nstck(stck);
// 		cout<<"\nodin kopiya drugogo i == : "<< (stck == nstck)<<"\n";
// 		stck.push(9);
// 		cout<<"\nv pervom bolshe elementov i > : "<< (stck > nstck)<<"\n";
// 	stop

// 	}


// 	////////////////////////////////////////////////////////////////////////////////////
// 	//queue

// 	// адаптер deque
// 	// доставать можно только первый, помещать только в конец
// 	// O(1)
	

// 	//Создайте очередь, которая содержит указатели на объекты типа Point,
// 	//при этом явно задайте базовый контейнер.
// 	//Измените значения первого и последнего элементов посредством front() и back()
// 	//Подумайте, что требуется сделать при уничтожении такой очереди?
// 	{
// 	queue<Point*, deque<Point*>> pq;
	
// 	Point* p1 = new Point(5,2);
//     Point* p2 = new Point(4,2);
//     Point* p3 = new Point(8,9);
	
// 	pq.push(p1);
// 	pq.push(p2);
//     pq.push(p3);
	
// 	printSQPQ(pq);
	
// 	Point* fp = pq.front();
//     fp->set_x(13);
//     fp->set_y(98);
	
// 	Point* bp = pq.back();
//     bp->set_x(50.0f);
//     bp->set_y(60.0f);
	
// 	 while (!pq.empty()) {
//         Point* ptr = pq.front();
//         delete ptr;
//         pq.pop();
// 	 }
	
// 	}
// 	////////////////////////////////////////////////////////////////////////////////////
// 	//priority_queue
	
// 	// адаптер vector
// 	// Вставка и удаление элементов реализованы посредством: std::make_heap(), std::push_heap() и std::pop_heap().
// 	// Представляет собой линеаризованное бинарное дерево.
// 	// Первый элемент всегда является максимальным.
// 	// Добавление и удаление элементов в общем случае производится с логарифмической сложностью.
	
// 	// make_heap преобразует последовательность в кучу за линейное время, не более 3*n сравнений
// 	// push_heap log(n)
// 	// pop_heap не более 2*log(n) сравнений
// 	// sort_heap не более n*log(n) сравнений
	
// 	//а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
// 	//б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
// 	//в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
// 	//		что сравнивается при вставке? 
	
// 	//если не делать свой компаратор, то у строк будут сравниваться адреса в памяти


// 	{	
// 		auto comparator = [](const char* a, const char* b) {
// 			return strcmp(a, b) < 0;
// 		};
// 		const char* slova[] = {"arbuz", "stol", "stul"};
// 		priority_queue<const char*, vector<const char*>, decltype(comparator)> priorQ(slova, slova + 3, comparator);
    
// 		printSQPQ(priorQ);

// 		stop
// 	}
	
	
// 	////////////////////////////////////////////////////////////////////////////////////
// 	//set
	
// 	// ассоциативный контейнер, значения уникальны и упорядочены
// 	// в основе лежит красно-черное дерево
// 	// ключ совпадает со значением
// 	// O(log(n))
	
// 	//a) создайте множество с элементами типа Point - подумайте, что необходимо определить
// 	//		в классе Point (и каким образом)
// 	//б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
// 	//в) попробуйте изменить любое значение...
// 	//г) Создайте два множества, которые будут содержать одинаковые значения
// 	//		типа int, но занесенные в разном порядке
// 	//д) Вставьте в любое множество диапазон элементов из любого другого
// 	//	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?)
// 	set<Point> points;
// 	points.insert(Point(5,2));
// 	points.insert(Point(4,2));
// 	points.insert(Point(8,9));
// 	pr(points, "Points");
// 	//нельзя изменять значения. Они упорядочены, а замена значения может сломать упорядоченность 
	
// 	set<int> chisla1 = {1,2,3,4,5};
// 	set<int> chisla2 = {3,2,5,1,4};
	
// 	array<int,5> chisla3 = {1,2,8,6,5};
// 	chisla1.insert(chisla3.begin(), chisla3.end()); // дубли не вставляются
// 	pr(chisla1, "set s dobavleniem"); 


// 	////////////////////////////////////////////////////////////////////////////////////
// 	//multiset
	
// 	multiset<int> chisla4 = {1,2,3,4,5,5,4,3,2,1};
// 	pr(chisla4, "multiset");



// 	////////////////////////////////////////////////////////////////////////////////////
// 	//map	
	
// 	// ассоциативный контейнер ключ-значение
// 	// ключи уникальны (по ним элементыы отсортированы)
// 	// в основе красно-черное дерево
// 	// O(log(n))
// 	// Для хранения пары ключ/значение используется шаблон структуры pair.

// 	/*
// 	template<typename T1 , typename T2> struct pair{
// 		T1 first;
// 		T2 second;
// 		pair ( const T1& t1 , const T2& t2 ) : first(t1),
// 		second (t2){ }
// } ;*/
// 	//а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
// 	//	при этом строки задаются строковыми литералами
// 	//б) заполните контейнер значениями посредством operator[] и insert()
// 	//в) распечатайте содержимое

// 	//е) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")
// 	map<const char*, int> famzp;
// 	famzp["Ivanova"] = 100;
// 	famzp.insert({"Timohina", 200});
// 	map<const char*, int>::iterator iter;
// 	for(iter = famzp.begin(); iter!=famzp.end(); iter++){
// 		cout<<iter->first <<"   " << iter->second<<"\n";
// 	}
// 	auto nodeHandler = famzp.extract("Ivanova");
// 	nodeHandler.key() = "Petrova";
// 	famzp.insert(std::move(nodeHandler));
// 	cout<<"\n";
// 	for(iter = famzp.begin(); iter!=famzp.end(); iter++){
// 		cout<<iter->first <<"   " << iter->second<<"\n";
// 	}
// 	stop
	


// 	////////////////////////////////////////////////////////////////////////////////////
// 	//multimap
	
// 	// Позволяет дублировать значения ключей, следовательно не определен operator[]. 
// 	// Элементы с одинаковыми ключами в контейнере хранятся в порядке занесения.
	

	
// 	//а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
// 	//		несколько русских значений - pair<string,string>, например: strange: чужой, странный...
// 	//б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью 
// 	//		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)
// 	//в) Выведите все содержимое словаря на экран
// 	//г) Выведите на экран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
// 	//		итераторов можно использовать методы lower_bound() и upper_bound()
	
// 	multimap<string,string> dict;
// 	dict.insert({"strange", "чужой"});
// 	dict.insert({"strange","странный"});
// 	dict.insert({"mine","мой"});
// 	dict.insert({"mine","шахта"});
	
// 	multimap<string, string>::iterator mmiter;
	
// 	for(mmiter = dict.begin(); mmiter!=dict.end(); mmiter++){
// 		cout<<mmiter->first <<"   " << mmiter->second<<"\n";
// 	}
// 	cout<<endl;
// 	string key = "strange";
// 	auto lower = dict.lower_bound(key);
// 	auto upper = dict.upper_bound(key);
// 	for(auto it = lower; it!=upper;it++){
// 		cout<<it->first <<"   " << it->second<<"\n";
// 	}
	

// //
// ///////////////////////////////////////////////////////////////////

// 	//Итераторы

// 	//Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
// 	//нужно перегрузить в классе Point. Создайте вектор, элементы которого 
// 	//являются копиями элементов set, но упорядочены по убыванию
	
// 	//нужно перегрузить operator<
// 	set<Point> sp;
// 	sp.insert(Point(5,2));
// 	sp.insert(Point(7,6));
// 	sp.insert(Point(1,3));
// 	sp.insert(Point(8,2));
// 	sp.insert(Point(4,2));
// 	vector<Point> pvec;
// 	for(auto it = sp.rbegin(); it != sp.rend(); it++){
// 		pvec.push_back(*it);
// 	}
	
// 	//Потоковые итераторы. С помощью ostream_iterator выведите содержимое
// 	//vector и set из предыдущего задания на экран.
// 	ostream_iterator<Point> out(cout, "; ");
// 	copy(sp.begin(), sp.end(), out);
// 	cout<<"\n\n";
// 	copy(pvec.begin(), pvec.end(), out);
		
		
		
// {
	
// 	deque<int> dq;
// 	vector<int> vec;
// 	list<int> lst;
// 	//Итераторы вставки. С помощью возвращаемых функциями:
// 	//back_inserter()   vec, deq, list
// 	//front_inserter()  deq, list
// 	//inserter()    для любых
// 	//итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
// 	//какие из итераторов вставки можно использовать с каждым контейнером.
// 	fill_n(back_inserter(vec), 3, 5);
// 	fill_n(front_inserter(dq), 2, 6);
// 	fill_n(inserter(lst, lst.begin()), 1, 7);

// }
// ///////////////////////////////////////////////////////////////////

// 	//Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

// 	// алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
// 	//(массив, vector, list...)
// 	//С помощью алгоритма for_each в любой последовательности с элементами любого типа
// 	//распечатайте значения элементов
// 	//Подсказка : неплохо вызываемую функцию определить как шаблон
// 	cout<<"\n\nfor_each point\n";
// 	for_each(sp.begin(), sp.end(), print<Point>);


// 	stop

// 	//С помощью алгоритма for_each в любой последовательности с элементами типа Point
// 	//измените "координаты" на указанное значение (такой предикат тоже стоит реализовать 
// 	//как шаблон) и выведите результат с помощью предыдущего предиката
	
// 	/*template <typename T>
// 	void modifyPoint(Point& point, T delta_x, T delta_y){
// 		point.set_x(point.get_x() + delta_x);
// 		point.set_y(point.get_y() + delta_y);
// 	}*/
// 	cout<<"\n\nfor_each point predicat\n";
// 	for_each(pvec.begin(), pvec.end(), [](Point& p){modifyPoint<float>(p,5,2);});
// 	for_each(pvec.begin(), pvec.end(), print<Point>);



// 	//С помощью алгоритма find() найдите в любой последовательности элементов Point
// 	//все итераторы на элемент Point с указанным значением.
// {	
// 	cout<<"\n\n";
// 	int counter = 0;
// 	auto it = pvec.begin();
// 	while(it!=pvec.end()){
// 		it = find(it, pvec.end(), Point(10,4));
// 		if(it!=pvec.end()){
// 			cout<<distance(pvec.begin(),it)<<"\n";
// 			it++;
// 			counter++;
// 		}
// 	}
		
// }	
// 	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Point. 
// 	////По умолчанию алгоритм сортирует последовательность по возрастанию.
// 	//Что должно быть определено в классе Point? operator<
// 	// Замечание: обобщенный алгоритм sort не работает со списком, так как
// 	//это было бы не эффективно => для списка сортировка реализована методом класса!!!
// 	cout<<"\n\n\n";
// 	sort(pvec.begin(), pvec.end());
// 	pr(pvec, "sorted vec");



// 	//Создайте глобальную функцию вида: bool Pred1_1(const Point& ), которая будет вызываться
// 	//алгоритмом find_if(), передавая в качестве параметра очередной элемент последовательности.
// 	//С помощью алгоритма find_if() найдите в любой последовательности элементов Point
// 	//итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
// 	//[-n, +m].

// 	/*bool Pred1_1(const Point& point){
// 		return (point.get_x() >= 8 && point.get_x()<=12) && (point.get_y() >= 8 && point.get_y()<=12); 
// 	}*/


// {
// 	constexpr int lower = 8;
// 	constexpr int upper = 12;
// 	auto it = find_if(pvec.begin(), pvec.end(), Pred1_1<lower,upper>);
// 	if(it!=pvec.end()){
// 		cout<<distance(pvec.begin(), it) << "\n\n";
// 	}
	
	
// }

// 	//С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
// 	//располагая прямоугольники по удалению центра от начала координат.
	
// 	/*double distanceFromOrigin(const Rect& rect){
// 		Vector center = rect.getCenterCoord();
// 		return sqrt(center.get_x()*center.get_x()+center.get_y()*center.get_y());
// 	}

// 	bool compareByDistance(const Rect& a, const Rect& b) {
// 		return distanceFromOrigin(a) < distanceFromOrigin(b);
// 	}*/
	
// 	vector<Rect> rects;
// 	rects.push_back(Rect(0, 0, 2, 2));    // Центр в 1 1 расстояние 1.41
//     rects.push_back(Rect(3, 4, 5, 6));    // Центр в 4 5 расстояние 6.40
//     rects.push_back(Rect(-1, -1, 1, 1));  // Центр в 0 0 расстояние 0
//     rects.push_back(Rect(10, 0, 12, 2));  // Центр в 11 1 расстояние 11.05
// 	sort(rects.begin(), rects.end(), compareByDistance);



// 	//transform
// 		//Напишите функцию, которая с помощью алгоритма transform переводит 
// 		//содержимое объекта string в нижний регистр.
// 		//Подсказка: класс string - это "почти" контейнер, поэтому для него
// 		// определены методы begin() и end()
		
// 		/*void tol(string& str){
// 			transform(str.begin(), str.end(), str.begin(), [](unsigned char s){return tolower(s);});
// 		}*/
		
// 		string fam = "IVANOV";
// 		cout<<"before transform tolower: "<<fam<<"\n\n";
// 		tol(fam);
// 		cout<<"after transform tolower: "<<fam<<"\n\n";

// 		//Заполните list объектами string. С помощью алгоритма transform сформируте
// 		//значения "пустого" set, конвертируя строки в нижний регистр
// {		
// 		list<string> slova = {"STOL", "StUl", "oKNo","ChaSHkA"};
// 		set<string> lowslova;
// 		transform(slova.begin(), slova.end(), inserter(lowslova, lowslova.begin()), [](const string& s){string t = s; tol(t); return t;});
// 		pr(lowslova, "set low strings");
// 		stop
// }
// 	// map
		
// 		//Сформируйте любым способом вектор с элементами типа string.
// 		//Создайте (и распечатайте для проверки) map<string, int>, который будет
// 		//содержать упорядоченные по алфавиту строки и
// 		//количество повторений каждой строки в векторе
// 		cout<<"\n\n";
// {		vector<string> vecstr = {"stol", "stul", "okno", "stol"};
// 		map<string, int> wordCount;
// 		for_each(vecstr.begin(), vecstr.end(), [&wordCount](const string& word){wordCount[word]++;});
// 		map<string, int>::iterator it;
// 		for(it = wordCount.begin(); it!=wordCount.end(); it++){
// 			cout<<it->first<<"   "<<it->second<<"\n";
// 		}
// }

// 	return 0;
// }

