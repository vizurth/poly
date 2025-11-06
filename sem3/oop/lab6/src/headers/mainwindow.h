#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QSlider>
#include <QScrollbar>
#include <QSpinBox>

class MainWindow : public QWidget {
	Q_OBJECT // MOC
public:
	MainWindow(QWidget* parent = nullptr);
	void addSomeWidget(const QString& type);

public slots: // используем слоты так как обычные можно вызывать только напрямую
	void connectAllWidgets();
	void removeAllConnect();

private:
	void setupUi();
	void connectPairOfWidgets(QWidget* w1, QWidget* w2, int currValue);
	void debugConnection();

	QVector<QWidget*> widgets; 
	bool connWidget;
};