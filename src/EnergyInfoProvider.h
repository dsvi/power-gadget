#ifndef ENERGYINFOPROVIDER_H
#define ENERGYINFOPROVIDER_H

class EnergyInfoProvider
{
public:
	static void init();
	static void terminate();

	/// Returns amout of physical chips
	static
	ui64 numPhysicalChips();
	/// Returns number of GPUs. Normally equal to number of physical chips
	static
	ui64 numGpus();
	/// In joules
	static
	double getTotalEnergyConsumedByChip(int physicalCpu = 0);
	/// In joules. 0 if not supported
	static
	double getTotalEnergyConsumedByGPU(int gpu = 0);
	/// In watts
	static
	double maximumChipPower(int physicalCpu = 0);
	/// In MHz
	static ui64 frequencyCPU(int cpu);
private:
	EnergyInfoProvider();
	~EnergyInfoProvider();
	static bool Initialized_;
	Q_DECLARE_TR_FUNCTIONS(EnergyInfoProvider);
};

#endif // ENERGYINFOPROVIDER_H
