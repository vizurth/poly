#include <QApplication>
#include <QVBoxLayout>
#include "headers/mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	MainWindow w;
	w.setWindowTitle("rect widget");
	w.show();

	return a.exec();
}

