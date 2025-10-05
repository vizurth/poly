#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv); // создаем обьект приложения

	MainWindow w; // создаем наш класс дочерний от QWidget
	
	w.show(); // отображаем виджет

	return a.exec(); // запуск приложения
} 
