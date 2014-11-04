#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Exception.h"
#include "EnergyInfoProvider.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::powerMeter(PowerMeter *pm)
{
	try{
		int maxChips = EnergyInfoProvider::numPhysicalChips();
		if (maxChips == 0){
			QLabel *err = new QLabel(tr("No compatible processors found"),this);
			ui->centralWidget->layout()->addWidget(err);
		}
		int maxGPUs = EnergyInfoProvider::numGpus();
		for (int i=0; i < maxChips; i++ ){
			PowerPlot *p = new PowerPlot(this);
			p->controlledChip(i);
			if (i < maxGPUs && EnergyInfoProvider::getTotalEnergyConsumedByGPU(i) > 0 )
				p->controlledGPU(i);
			p->startCollectingData(pm);
			ui->centralWidget->layout()->addWidget(p);
		}
	}
	catch(std::exception &e){
		QList<QWidget *> widgets = ui->centralWidget->findChildren<QWidget *>();
		foreach(QWidget * widget, widgets)
		{
				delete widget;
		}
		QLabel *err = new QLabel(Message(e), this);
		err->setTextInteractionFlags(Qt::TextSelectableByMouse);
		ui->centralWidget->layout()->addWidget(err);
		qDebug() << Message(e);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}
