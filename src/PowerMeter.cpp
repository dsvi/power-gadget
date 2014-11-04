#include "PowerMeter.h"
#include "EnergyInfoProvider.h"
#include "Exception.h"

PowerMeter::PowerMeter(QObject *parent) :
  QObject(parent)
{
	timerID_ = startTimer(samplingInterval_ * 1000);
}


void PowerMeter::timerEvent( QTimerEvent * )
{
	try{
		auto now = std::chrono::high_resolution_clock::now();
		auto dt = now - prevTime_;
		prevTime_ = now;
		qreal dtf = std::chrono::duration_cast<std::chrono::duration<double>>(dt).count();
		if (dtf <= 0)
			return;
		auto numMPs = EnergyInfoProvider::numPhysicalChips();
		processorsPower_.clear();
		processorsPower_.resize(numMPs);
		if (processorsPrevEnergy_.size() != numMPs){
			processorsPrevEnergy_.clear();
			processorsPrevEnergy_.resize(numMPs);
		}
		for ( int i = 0; i < numMPs; i++ ){
			// this once in a while returns 0?
			double e = EnergyInfoProvider::getTotalEnergyConsumedByChip(i);
			double &prevE = processorsPrevEnergy_[i];
			double de = e - prevE;
			prevE = e;
			if (de < 0)
				processorsPower_[i] = 0;
			else
				processorsPower_[i] = de / dtf;
		}

		auto numGPUs = EnergyInfoProvider::numGpus();
		GPUsPower_.clear();
		GPUsPower_.resize(numGPUs);
		if (GPUsPrevEnergy_.size() != numGPUs){
			GPUsPrevEnergy_.clear();
			GPUsPrevEnergy_.resize(numGPUs);
		}
		for ( int i = 0; i < numGPUs; i++ ){
			// this once in a while returns 0?
			double e = EnergyInfoProvider::getTotalEnergyConsumedByGPU(i);
			double &prevE = GPUsPrevEnergy_[i];
			double de = e - prevE;
			prevE = e;
			if (de < 0)
				GPUsPower_[i] = 0;
			else
				GPUsPower_[i] = de / dtf;
		}
		emit power(processorsPower_, GPUsPower_, dtf);
	}
	catch(std::exception &e){
		qWarning() << Message(e);
	}
}


void PowerMeter::samplingInterval(double sampligInterval)
{
	samplingInterval_ = sampligInterval;
	killTimer(timerID_);
	timerID_ = startTimer(sampligInterval * 1000);
}
