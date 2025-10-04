#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QSlider>
#include <QScrollbar>
#include <QSpinBox>

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	void addWidget(const QString& type);

public slots:
	void connectAllWidgets();

private:
	void setupUi();
	void connectPairOfWidgets(QWidget* w1, QWidget* w2);
	void debugConnection();

	QVector<QWidget*> widgets;
};