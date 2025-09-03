#include <cmath>
#include <cfloat>			// математические функции
#include <locale.h>	// функции, устанавливающие национальные кодировки символов
#include <iostream>	// Эта - часть библиотеки STL (Standard Template Library)
#include <limits>			// Полезные константы

using namespace std;		// Обеспечивает видимость имен STL

// подставляет точку останова для отладчика так как на g++ не работала 
#ifdef _MSC_VER
    #define stop __asm nop
#else
    #define stop do {} while(0)
#endif

// Эта макроподстановка упрощает установку точек останова

int n;	 // Глобальная переменная. По умолчанию она обнуляется.


namespace space	 // В пространстве имен - space
{
	int n = 1;	 // Объявлена переменная n
}				 // space определяет область видимости этой переменной

int main()
{
	// Выполняя программу по шагам, следите за значениями переменных  и интерпретируйте результат, объясняя себе
	// наблюдаемые значения. Обратите внимание на разную интерпретацию отладчиком  signed и unsigned типов данных.
	char c = 'A'; // 65 'A' 0x41 
	// В комментарии справа полезно проставить десятичное символьное и шестнадцатиричное 
	// значения переменной после выполнения указанной строки. Вы должны видеть их в окне Autos.
	c = 0x42;		// 66 'B' 0x42
	c = -1;		// -1 'я' 0xff
	c = CHAR_BIT;	 // Размер переменной типа char
	stop;

	// В комментариях напишите результат, возвращаемый оператором sizeof для
	// переменной типа wchar_t (ее размер)
	// wchat_t - используют для больших символов которые не помещаются в 1 байт, размер wchat_t от 2 до 4 байт(зависит от системы) т.е использует utf-16 или utf-32
	wchar_t cw = L'Ф';
	size_t ns = sizeof(cw); // 4 байта размер wchat_t
	cout << "size of wchar_t: " << ns << endl;

	unsigned char uc = 0x41; // так как 41(hex) в ASCII это A
	uc = 'B';
	uc = static_cast<unsigned char>(-1); // при -1 по правилу круга перекинемся на 255 так как unsigned char от 0 до 255
	stop;

	int i = 1;
	i = -1;
	cout << i << endl;

	stop;

	unsigned int ui = 1; // это unsigned int и вы присваем ему просто int нужно сделать явное привидение
	ui = static_cast<unsigned int>(-1);


	//	падает warning потому что диапозон short от -32768 до -32767 поэтому можем сделать cast
	//short s = 0xffff;	 // Здесь compiler генерирует warning. Измените код, чтобы убрать warning
	//s = short(0xffff);
	//s = 1;
	
	// Первый способ:
	short s = short(0xffff);
	s = 1;
	// Второй способ:
	//short s;
	//s = short(0xffff);
	//s = 1;



	//==== Измените код, чтобы убрать warning
	unsigned short us = 0xffff;
	us = 5;

	long l = 0xffffffff;
	l = -128;

	// Все эти константы вы должны найти в файле limits (float.h) и объяснить их смысл по комментариям
	l = FLT_MAX_EXP;	// максимальная степень для float
	l = DBL_MAX_EXP; // максимальная степень для DBL

	l = FLT_MANT_DIG; //количество бит мантиссы float
	l = DBL_DIG; // количество десятичных цифр точности double

	l = FLT_MANT_DIG; //количество бит мантиссы float
	l = DBL_MANT_DIG; //количество бит мантиссы double

	float f = -12.56f;	 // Здесь warning. Уберите его. нужно добавить f в конец литерала
	f = -1.e-27f;

	f = FLT_MAX; // максимальный float
	f = FLT_MIN; // минимальный float

	double d = 0.1234567890123456789123456789; 
	d = -0.123456789012345e+306;
	d = DBL_MAX; // максимальный double
	d = DBL_MIN; // минимальный double
	d = DBL_EPSILON; // Самая маленькая разность между двумя пер-ми типа double(погрешность)

	uc = static_cast<unsigned char>(~0);		 // Побитовое отрицание ~0 = 0xffffffff он пытается перевести int -> unsigned char нужно сделать cast
	i = ~0; // ошибки нет int -> int
	stop;
	// Раннее (при компиляции) или неявное приведение типов данных
	// Объясните те значения, которые вы наблюдаете в окне Autos. Определите порядок выполения присваиваний.
	//d = f = i = s = c = 1 / 3; // будет 0 так как в самом начале присваивается к char а он от -128 до 127
	//c = s = i = f = d = 100 / 3;
	//c = s = i = f = d = 10 / 3;
	//c = s = i = f = d = 1 / 3.;
	cout << "char: " <<  c << endl;
	cout << "float: " << f << endl;
	cout << "int: " << i << endl;
	cout << "double: " << d << endl;
	cout << "short: " << s << endl;

	// Пример "небрежного" использования неявного приведения типов.	 Объясните результаты.
	i = 256;
	c = static_cast<char>(i); 
	cout << "char unsafe cast 1: " << int(c) << endl; // в итоге будет 0 так как диапозон от -128 до 127 и при таком приведении произойдет потеря данных

	uc = 255;
	unsigned char u = 2, sum = uc + u;
	cout << "char unsafe cast 2: " << int(sum) << endl; // будет 1 так как 255 + 2 = 257 если возьмем mod 256 то получим 1, это примеры неявного приведения которые может повести за собой ошибки
	stop;

	// Явное приведение типов	Объясните разницу результатов в строках (3) и (4)
	//i = 100;
	//f = d = i / 3;							// (3) делит int на int и уже записывает в float и double
	//f = d = (double)i / 3;		// (4)
	//f = d = double(i) / 3;			// (4) в этим операциях одино из чисел double поэтому произойдет приведение и получим 33.333....
	//f = d = static_cast<double>(i) / 3;		// (4)
	stop;

	// Область действия, область видимости и время существования. В этом фрагменте фигурируют четыре
	//  переменных с одним и тем же именем n.  Одна - глобальная, вторая определена в своем пространстве имен,
	// третья - локальная внутри функции main(), четвертая - локальная внутри блока. 
	//  Обратите внимание, что глобальная переменная и та, что в пространстве имен space - объявлены вне функции main()
	// Определите, к какой из четырех переменных идет обращение, cформулируйте область действия и область видимости
	// каждой переменной. Для выполнения задания рекомендуется пользоваться  закладкой "Watches" или "Locals" окна
	// "Autos". Подсказка: В окно "Watches" можно поместить сразу все переменные (n, ::n, и space::n)
	//n = 100; // глобальная ::n
	//space::n = 200; // из простраства имен space
	//n++; // глобальная ::n
	//int n;		//Где живет эта переменная ? в main()
	//n = 10; // n в main()
	//::n++; // глобальная ::n

	//{//  Начало блока
	//	int n;		// Эта переменная живет внутри блока
	//	n = -1; // n внутри блока
	//	n++; // n внутри блока
	//	::n++; // глобальная ::n
	//	space::n++; // из пространства имен space::n
	//}		// Конец блока n в блоке уничтожилась
	//n--; // n в main
	//::n--; // глобальная ::n
	//space::n--; // пространство имен space::n

	// Спецификатор класса памяти - static Выполняя задание по шагам, обратите внимание на  разное поведение
	// переменных nLoc и nStat
	{
	Again:
		int outer;
		for (int i = 0; i < 5; i++)
		{
			static int nStat;
			{
				int nLoc = 0;
				nLoc++;		nStat++; //nLoc постоянно равен 1
			}
			outer = nStat;
			cout << "i = " << i
                 << " | nStat = " << nStat
                 << " | outer = " << outer << endl;
		}
		cout << "--- End of for loop ---" << endl;
		if (outer < 10)
			goto Again;
	}
	// Перечисления - enum. Обратите внимание на явную и неявную инициализацию констант
	enum RANK
	{
		One, // 0
		Two, // 1
		Three, // 2
		Four, // 3
		Jack = 6, // 6(явно)
		Queen, // 7
		Ace = Queen + 3, // 10
		Joker = 20 // 20
	};
	typedef RANK RANG; // пишем псевдоним alias

	RANG r = Jack; // 6
	if (r == Jack)
		r = Queen; // 7

	if (r == Queen)
	{
		// Любой целочисленной переменной можно присвоить enum-переменную 
		int i = r; // i = 7
		r = RANK(i++);		// Обратное преобразование надо указывать явно // 7
		i = r; // i = 7
		r = RANK(++i); // 8
		i = r; // 8
	}
	RANK rr = Ace; // 10
	cout << "rr = Ace: " << rr << endl;
	stop;
	//	Логический тип bool.	Выполняя задание по шагам, следите за значениями переменной b
	{
		int n = 127;
		bool b = n != 0; // true
		b = n == 0; // false
		b = n > 0; // true
		b = n <= 0; // false
		b = n > 1; // true

		int num = static_cast<int>(b); // bool -> int, true -> 1
		if (b)
			cout << "\n\t My flag is: true" << "   or: " << b
			<< "\n\t Conversion to int: " << num << endl;
		b = n == num; // false
		cout << "\n\t Now the flag is: false" << "   or: " << b;
	}

	//	Модификатор const
	const double pi = acos(-1.);
	double space_permiability = 4.e-7 * pi;		//	Магнитная проницаемость пустоты
	cout << "space_permiability: " << space_permiability << endl;
	const int dozen = 12;
	int var = dozen;
	cout << "var: " << var << endl;

	//	Раскомментируйте следующую строчку и объясните ошибку компиляции (l-value означает left value)
		//dozen = 1; // так как dozen const int мы не можем его изменить(так как const это rvalue)
	//	Директивы условной трансляции.(обрабатываются до компиляции, до этапа препроцессовра) Объясните значение, которое принимает переменная version.
	//	Что нужно сделать для того, чтобы результат был другим? (можно просто удалить define)
	const char* version;
//#define _MSVER400
#define _MSVER311

#if defined _MSVER400
	version = "version 4.00";
#elif defined _MSVER311
	version = "version 3.11";
#else
	version = "version Unknown";
#endif

#define _MSVER311


	cout << endl << version;
	stop;

	//В окне ClassView или Solution Explorer поставьте фокус на имя проекта дайте команду Project/Properties.
	//	В диалоге Property Pages щелкните на папке Configuration Properties, убедитесь, что в разделе Code Generation
	//	установлена константа компиляции _DEBUG. Создайте директивы препроцессора и код С++, которые
	//	в зависимости от действующей конфигурации проекта (_DEBUG или NDEBUG) выводят соответствующее
	//	сообщение. Измените конфигурацию проекта (в диалоге Property Pages) и проверьте ваш код.
	//	Чтобы изменить конфигурацию пользуйтесь кнопкой Configuration Manager

	//	Простейшие циклы. Объясните суть происходящего. 
	{
		// Цикл while надо использовать, когда неизвестно количество итераций (повторений) цикла.
		cout << "\n\nGradually eat out all the units:\n\n";
		unsigned short us = 0xff;
		while (us)
		{
			cout << hex << us << endl;
			us &= us - 1;
		}
		cout << hex << us << "\nDone\n";
		// Цикл for надо использовать, когда известно количество итераций и/или есть код подготовки.
		// Подсказка: odd - нечетое, even - четное.
		cout << "\n\nShow even-odd:\n\n";
		for (int i = 0; i < 10; i++)
		{
			if (i & 1)
				cout << i << " - odd\n";
			else
				cout << i << " - even\n";
		}
		// Когда нужно использовать цикл do-while? (когда гарантированно хотим получить как минимум одну итерацию)
		char c = ' ';
		do
		{
			if (c == 'a')
				cout << "\nAction is a delegate type in C#";
			else if (c == 'b')
				cout << "\nbreak is one of the 'leave' statements in all C-like languages";
			else if (c == 'c')
				cout << "\ncontinue is a 'go on' statement in all C-like languages";
			else if (c == 'd')
				cout << "\ndo-while is a rarely used loop statement in all C-like languages";
			else
				cout << "\nPlease read the rules of this loop";
			cout << "\n\nEnter chars: a, b, c, d (q - to quit):\n\n";
			cin >> c;
			if (c == 'q')
				cout << "\nI am going to leave the loop\n";
		} while (c != 'q');
		stop;
	}
	//	Логические условные операторы и циклы. Функция y = f(x) задана графиком
	//	Напишите фрагмент кода, который с шагом 0.1 вычисляет y = f(x)  и выводит в консольное окно значения x и y. 
	//	  y
	//	  | 
	//	 2|__________
	//	  |         /\
	//	  |        /  \
	//	  |       /    \
	//	  |______/      \__________  x
	//	  0      1   2   3
	//	 Реалируйте 2 варианта этого алгоритма:   1. Используйте операторы if	 2. Используйте тернарные операции    ? :
	cout << "Способ через if: " << endl;
	{
		double x, y;

		for (x = 0.0; x <= 4.0; x += 0.1){
			if (x < 1.0 || x > 3.0)
        y = 0;
      else if (x < 2.0)
        y = 2*(x - 1);
      else
        y = 2*(3 - x);

			cout << "(x, y) = (" << x << ", " << y << ")" << endl;
		}
	}
	cout << '\n';
	//cout << "Способ через ?: " << endl;
	//{
	//	double x, y;

	//	for (x = 0.0; x <= 4.0; x += 0.1){
	//		y = (x < 1.0 || x > 3.0) ? 0 :
	//		(x < 2.0) ? 2*(x - 1) : 2*(3 - x);

	//		cout << "(x, y) = (" << x << ", " << y << ")" << endl;
	//	}
	//}

	//	Напишите фрагмент, который с помощью for и switch реализует следующую логику. Если пользователь ввел:
	//	символ 'a',   ваш алгоритм выводит  "Ok" (в кавычках)
	//	символ 'b',   ваш алгоритм выводит  Bell (alert - звуковой сигнал)
	//	символ 'с',   ваш алгоритм выводит  число, которое равно количеству введенных символов
	//	символ 'Esc', ваш алгоритм выводит  "to quit use 'q'"
	//	символ 'q',   ваш алгоритм выводит  "Bye" и выходит из цикла ввода
	{
		char c;
		int count = 0;
		cout << "Введите символы и смотрите результат:\n";
    cout << "символ 'a' - выводит \"Ok\"\n";
    cout << "символ 'b' - выводит звуковой сигнал (Bell)\n";
    cout << "символ 'c' - выводит количество введенных символов\n";
    cout << "символ 'Esc' - выводит \"to quit use 'q'\"\n";g
    cout << "символ 'q' - выводит \"Bye\" и выходит из цикла\n\n";
		for (;;){
			cin >> c;
			count++;
			switch (c){
				case 'a':
					cout << "\"OK\"" << endl;
					break;
				case 'b':
					cout << '\a';
					break;
				case 'c':
					cout << "\ncount: " << count << endl;
					break;
				case 27: // esc in ASCII
					cout << "\nto quit use 'q'" << endl;
					break;
				case 'q':
					cout << "\nBye\n";
					break;
				default:
                cout << "Неподдерживаемый символ" << endl;
			}
			if (c == 'q') {
        break;
      }
		}
	}
	cout << '\n';

	//	Побитовые операции:  |, &, ~, ^ и сдвиги >>, <<
	//	Поменяйте местами байты переменной flags и выведите результат в консолное окно unsigned short flags = 0xaabb;
	//	Ваш код
	unsigned short flags = 0xaabb;

	cout << "До: " << hex << flags << endl;

	flags = static_cast<unsigned short>((flags << 8) | (flags >> 8));

	cout << "После: " << hex << flags << endl;

	//	Для вывода в шестнадцатеричном виде используйте  cout <<"\n bits = " << hex << flags;

	//	В переменной unsigned char byte = 0x26; 
        //      - установите в единицу 3-й бит (счет от нуля). Выведите результат.
	//	- инвертируйте два младших бита. Выведите результат. 
        //      - обнулите 4 младших бита. Выведите результат.
	//	cout <<endl << hex << flags << endl<<dec<<flags;    

	unsigned char byte = 0x26;
	byte |= (1 << 3);
	cout << "установите в единицу 3-й бит: " << hex << static_cast<int>(byte) << endl;

	byte ^= 0x03;
	cout << "инвертация двух младших битов: " << hex << static_cast<int>(byte) << endl;

	byte &= 0xf0;
	cout << "обнулим 4 младших бита: " << hex << static_cast<int>(byte) << endl;

	cout << "\nflags: hex = 0x" << hex << flags << ", dec = " << dec << flags << endl;

	cout << "\n\n";
}	// Конец функции main()
