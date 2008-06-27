#ifndef TunnerMonitor_hxx
#define TunnerMonitor_hxx

#include <vector>
#include <CLAM/DataTypes.hxx>
#include "PeakDataSource.hxx"

#include <CLAM/PortMonitor.hxx>

class TunnerMonitor : public CLAM::PortMonitor<std::pair<CLAM::TData,CLAM::TData> >, public PeakDataSource
{
	const char* GetClassName() const { return "Tunner"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * positionData()
	{
		_fasor = FreezeAndGetData();
		UnfreezeData();
		return &(_fasor.first);
	}
	const CLAM::TData * magnitudeData()
	{
		return &(_fasor.second);
	}
	unsigned nBins() const
	{
		return 1;
	}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	std::pair<CLAM::TData,CLAM::TData> _fasor;
};


#endif//TunnerMonitor_hxx
