#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PowerPlot.h"
#include "PowerMeter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void powerMeter(PowerMeter *pm);

private:
	Ui::MainWindow *ui;


	// QWidget interface
protected:
	void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
