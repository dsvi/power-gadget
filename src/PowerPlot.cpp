#include "PowerPlot.h"
#include "ui_PowerPlot.h"
#include "Exception.h"
#include "EnergyInfoProvider.h"
#include "EngFormattedValue.h"

class CpuCurve: public QwtPlotCurve
{
public:
		CpuCurve( const QString &title ):
				QwtPlotCurve( title )
		{
				setRenderHint( QwtPlotItem::RenderAntialiased );
		}

		void setColor( const QColor &color )
		{
				QColor c = color;
				c.setAlpha( 120 );

				setPen( c );
				setBrush( c );
		}
};

PowerPlot::PowerPlot(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PowerPlot)
{
	ui->setupUi(this);
	try{
		plot_ = new QwtPlot(this);
		plot_->setFrameStyle(QFrame::NoFrame);
		QwtPlotGrid *grid = new QwtPlotGrid();
		grid->attach(plot_);
		//grid_.setMajorPen(QPen( Qt::gray, 1 ));
		grid->setMajorPen(QPen( plot_->palette().color(QPalette::Disabled, QPalette::WindowText), 1));
		QwtPlotCanvas *canvas = dynamic_cast<QwtPlotCanvas*>( plot_->canvas() );
		canvas->setFrameStyle(QFrame::NoFrame);
		plot_->setMinimumSize(300, 50);
		//THROW(tr("fuck me!"));
		QFont f = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
		plot_->setAxisScale( QwtPlot::yLeft, 0, EnergyInfoProvider::maximumChipPower() );
		plot_->setAxisMaxMajor(QwtPlot::yLeft, 4);
		plot_->setAxisScale( QwtPlot::xBottom, 0, timeWindowSec_ );
		f.setPointSize(8);
		plot_->setAxisFont(QwtPlot::yLeft, f);
		plot_->setAxisFont(QwtPlot::xBottom, f);
		this->layout()->addWidget(plot_);
	}
	catch(std::exception &e){
		fail(e);
	}
}

PowerPlot::~PowerPlot()
{
	delete ui;
}

void PowerPlot::controlledChip(int chipNdx)
{
	ASSERT(chipNdx >= 0);
	controlledMP_ = chipNdx;
}

void PowerPlot::controlledGPU(int gpuNdx)
{
	controlledGPU_ = gpuNdx;
}

void PowerPlot::startCollectingData(PowerMeter *pm)
{
	if (isFailed())
		return;
	try{
		curveMP_ = new CpuCurve( "Total" );
		curveMP_->attach( plot_ );
		curveMP_->setColor( Qt::blue );

		curveGPU_ = new CpuCurve( "GPU" );
		curveGPU_->attach( plot_ );
		curveGPU_->setColor( Qt::green );

		connect(
			pm, SIGNAL(power(std::vector<double>&,std::vector<double>&,double)),
			this, SLOT(onPower(std::vector<double>&,std::vector<double>&,double)));
	}
	catch(std::exception &e){
		fail(e);
	}
}

void PowerPlot::fail(std::exception &e)
{
	if (plot_){
		delete plot_;
		plot_ = nullptr;
	}
	QList<QWidget *> widgets = findChildren<QWidget *>();
	foreach(QWidget * widget, widgets)
	{
			delete widget;
	}
	QLabel *err = new QLabel(Message(e), this);
	err->setTextInteractionFlags(Qt::TextSelectableByMouse);
	this->layout()->addWidget(err);
}

void PowerPlot::onPower(std::vector<double> &powerMPs, std::vector<double> &powerGPUs, double dt)
{
	if (isFailed())
		return;
	try{
		if (powerMPs.size() <= controlledMP_ )
			return;
		auto powerMP = powerMPs[controlledMP_];
		float maxTime = timeWindowSec_;
		auto slideOldAndAddNew = [dt, maxTime](QVector<QPointF> &v, qreal val){
			auto clearTo = v.begin();
			for(auto i = v.begin(); i < v.end(); ++i){
				qreal &rx = (*i).rx();
				rx -= dt;
				if (rx <= 0)
					clearTo = i;
			}
			v.erase(v.begin(), clearTo);
			v.push_back(QPointF(maxTime,val));
		};
		slideOldAndAddNew(logMP_,powerMP);
		curveMP_->setSamples(logMP_);
		QString status = QString(tr("Total: %1W").arg(EngFormattedValue::f(powerMP, 3)));
		if (powerGPUs.size() > controlledGPU_){
			auto powerGPU = powerGPUs[controlledGPU_];
			slideOldAndAddNew(logGPU_,powerGPU);
			curveGPU_->setSamples(logGPU_);
			status += QString(tr("  GPU: %1W").arg(EngFormattedValue::f(powerGPU, 3)));
		}
		ui->freq->setText(status);
		if (isVisible())
			plot_->replot();
	}
	catch(std::exception &e){
		fail(e);
	}
}

