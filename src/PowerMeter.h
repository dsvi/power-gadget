#ifndef POWERMETER_H
#define POWERMETER_H

#include <QObject>


class PowerMeter : public QObject
{
	Q_OBJECT
public:
	explicit PowerMeter(QObject *parent = 0);
	Q_PROPERTY(double samplingInterval READ samplingInterval WRITE samplingInterval )

	double samplingInterval() const;
	/// interval, over which the power is avergaed. in seconds. default is 1
	void samplingInterval(double samplingInterval);

signals:
	/// index in vector is the number of MicroProcessor or GPU, dt - time interval for which power was averaged
	void power(std::vector<double> &powerMP, std::vector<double> &powerGPU, double dt);
public slots:


	// QObject interface
protected:
	void timerEvent(QTimerEvent *);
private:
	// in seconds
	double samplingInterval_ = 1;
	std::vector<double> processorsPrevEnergy_;
	std::vector<double> GPUsPrevEnergy_;
	std::vector<double> processorsPower_;
	std::vector<double> GPUsPower_;
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	TimePoint        prevTime_;
	int timerID_;
};

inline
double PowerMeter::samplingInterval() const
{
	return samplingInterval_;
}

#endif // POWERMETER_H
