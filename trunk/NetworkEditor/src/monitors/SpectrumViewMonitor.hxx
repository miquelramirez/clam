#ifndef SpectrumViewMonitor_hxx
#define SpectrumViewMonitor_hxx

#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/SpecTypeFlags.hxx>
#include <CLAM/Spectrum.hxx>
#include "FloatArrayDataSource.hxx"

class SpectrumViewMonitor :
	public CLAM::PortMonitor<CLAM::Spectrum>,
	public CLAM::VM::FloatArrayDataSource
{
	const char* GetClassName() const { return "SpectrumView"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string a;
		return a;
	}
	const CLAM::TData * frameData()
	{
		_spectrum = FreezeAndGetData();
		if ( !_spectrum.HasMagBuffer())
		{
			CLAM::SpecTypeFlags flags;
			flags.bMagPhase=true;
			_spectrum.SetType(flags);
			_spectrum.SetTypeSynchronize(flags);
			_spectrum.ToLinear();
		}
		const CLAM::Array<CLAM::TData> & data = _spectrum.GetMagBuffer();
		_size = data.Size();
		if (_size==0) return 0;
		return &data[0];
	}
	void release()
	{
		UnfreezeData();
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
	CLAM::Spectrum _spectrum;
};


#endif//SpectrumViewMonitor_hxx
