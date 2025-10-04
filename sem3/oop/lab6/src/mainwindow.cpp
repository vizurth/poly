#include "headers/mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QScrollbar>
#include <QSpinBox>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
	setupUi();
}
void MainWindow::setupUi(){
	auto layout = new QVBoxLayout(this);

	auto addSliderButton = new QPushButton("add QSlider");
	auto addLabelButton = new QPushButton("add QLabel");
	auto addScrollBarButton = new QPushButton("add QScrollBar");
	auto addSpinBoxButton = new QPushButton("add QSpinBox");
	
	auto connectButton = new QPushButton("connect all widgets");

	layout->addWidget(addSliderButton);
	layout->addWidget(addLabelButton);
	layout->addWidget(addScrollBarButton);
	layout->addWidget(addSpinBoxButton);

	layout->addWidget(connectButton);

	connect(addSliderButton, &QPushButton::clicked, this, [this]() {addWidget("QSlider");});
	connect(addLabelButton, &QPushButton::clicked, this, [this]() {addWidget("QLabel");});
	connect(addScrollBarButton, &QPushButton::clicked, this, [this]() {addWidget("QScrollBar");});
	connect(addSpinBoxButton, &QPushButton::clicked, this, [this]() {addWidget("QSpinBox");});

	connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectAllWidgets);
}

void MainWindow::addWidget(const QString& type){
	QWidget* newWidget = nullptr;
	const int min = 0;
	const int max = 100;

	if (type == "QSlider") {
		auto slider = new QSlider(Qt::Horizontal, this);
		slider->setRange(min, max);
		newWidget = slider;
	} else if (type == "QLabel") {
		newWidget = new QLabel("Label: 0", this);
	} else if (type == "QScrollBar") {
		auto scrollBar = new QScrollBar(Qt::Horizontal, this);
		scrollBar->setRange(min, max);
		newWidget = scrollBar;
	} else if (type == "QSpinBox"){
		auto spinBox = new QSpinBox(this);
		spinBox->setRange(min, max);
		newWidget = spinBox;
	}

	if (newWidget) {
		widgets.append(newWidget);
		if (layout()){
			layout()->addWidget(newWidget);
		}
		newWidget->show();
	}
}
void MainWindow::connectPairOfWidgets(QWidget* w1, QWidget* w2){
	QString class1 = w1->metaObject()->className();
	QString class2 = w2->metaObject()->className();


	if (class1 == "QSlider" || class1 == "QScrollBar" || class1 == "QSpinBox"){
		if (class2 == "QLabel"){
			connect(w1, SIGNAL(valueChanged(int)), w2, SLOT(setNum(int)), Qt::UniqueConnection);
		} else if (class2 == "QSlider" || class2 == "QScrollBar" || class2 == "QSpinBox"){
			connect(w1, SIGNAL(valueChanged(int)), w2, SLOT(setValue(int)), Qt::UniqueConnection);
		}
	}
}

void MainWindow::connectAllWidgets(){
	qDebug() << "Connnection all widgets...";

	for (int i = 0; i < widgets.size(); ++i){
		for (int j = i + 1; j < widgets.size(); ++j){
			QWidget* w1 = widgets[i];
			QWidget* w2 = widgets[j];

			connectPairOfWidgets(w1, w2);
			connectPairOfWidgets(w2, w1);
		}
	}

	qDebug() << "All widgets connected.";
	debugConnection();
}

void MainWindow::debugConnection(){
	qDebug() << "--- Connnection Debugging ---";
	int totalConnections = 0;
	for (QWidget* w : widgets) {
		qDebug() << "Widget" << w->metaObject()->className() << "at" << w << "has connection.";
		QObject::dumpObjectInfo();
	}
	qDebug() << "-----------------------------";
}