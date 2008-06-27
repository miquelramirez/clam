#ifndef PolarChromaPeaksMonitor_hxx
#define PolarChromaPeaksMonitor_hxx

#include <vector>
#include <CLAM/DataTypes.hxx>
#include "PeakDataSource.hxx"

#include <CLAM/PortMonitor.hxx>

class PolarChromaPeaksMonitor : public CLAM::PortMonitor<std::vector<std::pair<CLAM::TData,CLAM::TData> > >, public PeakDataSource
{
	const char* GetClassName() const { return "PolarChromaPeaks"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * positionData()
	{
		_peaks = FreezeAndGetData();
		UnfreezeData();
		_size = _peaks.size();
		_pos.resize(_size);
		_value.resize(_size);
		for (unsigned i=0; i<_size; i++)
		{
			_pos[i] = _peaks[i].first;
			_value[i] = _peaks[i].second;
		}
		return &_pos[0];
	}
	const CLAM::TData * magnitudeData()
	{
		return &_value[0];
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
	std::vector<std::pair<CLAM::TData,CLAM::TData> > _peaks;
	std::vector<CLAM::TData> _pos;
	std::vector<CLAM::TData> _value;
};


#endif//PolarChromaPeaksMonitor_hxx
