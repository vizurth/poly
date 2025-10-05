#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QApplication>


class MainWindow : public QWidget {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
		setWindowTitle("Hello World App");

		setFixedSize(400, 200);
		QLabel* label = new QLabel("Hello World!", this);
		label->setGeometry(150, 50, 100, 30);

	    QPushButton* button = new QPushButton("Exit", this);
	    button->setGeometry(150, 100, 100, 30);

		QObject::connect(button, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
	}
 };