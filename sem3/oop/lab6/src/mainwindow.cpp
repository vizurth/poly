#include "headers/mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QScrollbar>
#include <QSpinBox>
#include <vector>
#include <iostream> 
#include <string> 


MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
	setupUi();
}

void MainWindow::setupUi(){
	auto layout = new QVBoxLayout(this);
	std::vector<std::string> buttonNames = {"QSlider", "QLabel", "QScrollBar", "QSpinBox", "connect"};

	for (const auto& name : buttonNames) {
		if (name != "connect"){
			QString buttonText = QString("add") + QString::fromStdString(name);
			auto addButton = new QPushButton(buttonText);
			layout->addWidget(addButton);
			connect(addButton, &QPushButton::clicked, this, [this, name]() {addWidget(QString::fromStdString(name));});
		} else {
			auto connectButton = new QPushButton("connect all widgets");
			layout->addWidget(connectButton);
			connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectAllWidgets);
		}
	}
}

void MainWindow::addWidget(const QString& type){
	QWidget* newWidget = nullptr;
	const int min = 0;
	const int max = 100;
	int initialValue = 0;

	// если уже есть виджеты и соединение активно — взять значение первого
	if (connWidget && !widgets.isEmpty()) {
		QWidget* first = widgets.first();

		if (auto slider = qobject_cast<QSlider*>(first))
			initialValue = slider->value();
		else if (auto spinBox = qobject_cast<QSpinBox*>(first))
			initialValue = spinBox->value();
		else if (auto scroll = qobject_cast<QScrollBar*>(first))
			initialValue = scroll->value();
		else if (auto label = qobject_cast<QLabel*>(first))
			initialValue = label->text().toInt();
	}


	if (type == "QSlider") {
		auto slider = new QSlider(Qt::Horizontal, this);
		slider->setRange(min, max);
		slider->setValue(initialValue);
		newWidget = slider;
	} else if (type == "QLabel") {
		newWidget = new QLabel(QString::number(initialValue), this);
	} else if (type == "QScrollBar") {
		auto scrollBar = new QScrollBar(Qt::Horizontal, this);
		scrollBar->setRange(min, max);
		scrollBar->setValue(initialValue);
		newWidget = scrollBar;
	} else if (type == "QSpinBox"){
		auto spinBox = new QSpinBox(this);
		spinBox->setRange(min, max);
		spinBox->setValue(initialValue);
		newWidget = spinBox;
	}

	if (connWidget) {
		for (QWidget* existingWidget : widgets) {
			connectPairOfWidgets(newWidget, existingWidget);
			connectPairOfWidgets(existingWidget, newWidget);
		}
	} 

	widgets.append(newWidget);
	if (layout()){
		layout()->addWidget(newWidget);
	}
	newWidget->show();
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
	connWidget = true;
	qDebug() << "Connecting all widgets...";

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
    qDebug() << "--- Connection Debugging ---";
    for (QWidget* w : widgets) {
        qDebug() << "Widget:" << w
                 << "(" << w->metaObject()->className() << ")";
        // выводим objectName, если есть
        qDebug() << "  ObjectName:" << w->objectName();

        // выводим сигналы и слоты
        w->dumpObjectInfo();

        // проверяем наличие активных соединений
        const QMetaObject* mo = w->metaObject();
        int signalCount = mo->methodCount();
        qDebug() << "  Total methods in metaobject:" << signalCount;
    }
    qDebug() << "-----------------------------";
}
