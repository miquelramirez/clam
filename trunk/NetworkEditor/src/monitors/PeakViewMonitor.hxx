#ifndef PeakViewMonitor_hxx
#define PeakViewMonitor_hxx

#include <CLAM/SpectralPeakArray.hxx>

#include "PeakDataSource.hxx"

#include <CLAM/PortMonitor.hxx>
class PeakViewMonitor : public CLAM::PortMonitor<CLAM::SpectralPeakArray>, public PeakDataSource
{
	const char* GetClassName() const { return "PeakView"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a("A");
		return a;
	}
	const CLAM::TData * positionData()
	{
		_peaks = FreezeAndGetData();
		UnfreezeData();
		if ( !_peaks.HasFreqBuffer()) return 0;
		const CLAM::Array<CLAM::TData> & data = _peaks.GetFreqBuffer();
		_size = data.Size();
		if (!_size) return 0;
		return &data[0];
	}
	const CLAM::TData * magnitudeData()
	{
		if ( !_peaks.HasMagBuffer()) return 0;
		const CLAM::Array<CLAM::TData> & data = _peaks.GetMagBuffer();
		return &data[0];
	}
	unsigned nBins() const
	{
		return _size;
	}
	unsigned nAspects() const
	{
		return 2;
	}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
	CLAM::SpectralPeakArray _peaks;
};


#endif//PeakViewMonitor_hxx
