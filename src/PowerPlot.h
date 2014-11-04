#ifndef POWERPLOT_H
#define POWERPLOT_H

#include <QWidget>
#include "PowerMeter.h"

namespace Ui {
class PowerPlot;
}

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

class CpuCurve;

class PowerPlot : public QWidget
{
	Q_OBJECT

public:
	explicit PowerPlot(QWidget *parent = 0);
	~PowerPlot();

	void controlledChip(int chipNdx);
	/// can be -1, which means no controlled GPU
	void controlledGPU(int gpuNdx);
	/// subscribe to pm
	void startCollectingData(PowerMeter *pm);

private:
	Ui::PowerPlot *ui;
	QwtPlot       *plot_ = nullptr;
	constexpr static const qreal timeWindowSec_ = 120;
	int            controlledMP_;
	int            controlledGPU_; // -1 - no GPU
	CpuCurve      *curveMP_;
	CpuCurve      *curveGPU_;
	QVector<QPointF> logMP_;
	QVector<QPointF> logGPU_;
	void fail(std::exception &e);
	inline
	bool isFailed(){
		return plot_ == nullptr;
	}

private slots:
	void onPower(std::vector<double> &powerMP, std::vector<double> &powerGPU, double dt);
};

#endif // POWERPLOT_H
