#include "EnergyInfoProvider.h"
extern "C"{
#include "rapl.h"
}
#include "Exception.h"
#include "AtExit.h"

bool EnergyInfoProvider::Initialized_ = false;

void EnergyInfoProvider::init()
{
	ASSERT(!Initialized_);

	if(1)
	{
//		auto uid = getuid();
//		AtExit resetUID([uid](){
//			setuid(uid);
//		});

		auto err = setuid(0);
		if (err){
			THROW(tr(
				"Can't get root privileges, required for playing with /dev/cpu/.../cpuid & msr\n"
				"Do 'sudo setcap cap_sys_rawio,cap_setuid+eip' on this executable file"));
		}

		err = std::system("/sbin/modprobe cpuid");
		if (err){
			EXCEPTION(e);
			e << tr("Can't '/sbin/modprobe cpuid' err code: ") << err;
			throw e;
		}
		err = std::system("/sbin/modprobe msr");
		if (err){
			EXCEPTION(e);
			e << tr("Can't '/sbin/modprobe msr' err code: ") << err;
			throw e;
		}
		// shouldn't need this
		int cpuNum = 0;
		QString fCpuid("/dev/cpu/%1/cpuid");
		QString fMsr("/dev/cpu/%1/msr");
		mode_t mode = S_IROTH | S_IWOTH | S_IRUSR | S_IWUSR;
		errno = 0;
		do{
			auto err = chmod(fCpuid.arg(cpuNum).toUtf8(), mode);
			if (err)
				break;
			err = chmod(fMsr.arg(cpuNum).toUtf8(), mode);
			if (err)
				break;
			cpuNum++;
		}
		while ( !errno );
		if (errno != ENOENT){
			EXCEPTION(e);
			e << tr("Can't change /dev/cpu/.../cpuid or msr file permissions: ");
			e << strerror(errno);
			throw e;
		}
		errno = 0;
	}

	if (init_rapl()){
		terminate_rapl();
		THROW(tr("Error initializing rapl library"));
	}

	Initialized_ = true;
}

void EnergyInfoProvider::terminate()
{
	ASSERT(Initialized_);
	Initialized_ = false;
	auto err = terminate_rapl();
	ASSERT(err == 0);
}

ui64 EnergyInfoProvider::numPhysicalChips()
{
	ASSERT(Initialized_);
	return get_num_rapl_nodes_pkg();
}

ui64 EnergyInfoProvider::numGpus()
{
	ASSERT(Initialized_);
	return get_num_rapl_nodes_pp1();
}

double EnergyInfoProvider::getTotalEnergyConsumedByChip(int physicalCpu)
{
	ASSERT(Initialized_);
	double e = 0;
	get_pkg_total_energy_consumed(physicalCpu, &e);
	return e;
}

double EnergyInfoProvider::getTotalEnergyConsumedByGPU(int gpu)
{
	ASSERT(Initialized_);
	double e = 0;
	get_pp1_total_energy_consumed(gpu, &e);
	return e;
}

double EnergyInfoProvider::maximumChipPower(int physicalCpu)
{
	ASSERT(Initialized_);
	pkg_rapl_parameters_t param;
	auto err = get_pkg_rapl_parameters(physicalCpu, &param);
	if (err)
		THROW(tr("Can't get maximum TDP for the chip"));
	return param.thermal_spec_power_watts;
}

ui64 EnergyInfoProvider::frequencyCPU(int cpu)
{
	ASSERT(Initialized_);
	ui64 freq;
	get_pp0_freq_mhz(cpu, &freq);
	return freq;
}
