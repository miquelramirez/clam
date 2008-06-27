#ifndef BufferOscilloscopeMonitor_hxx
#define BufferOscilloscopeMonitor_hxx


#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>


//TODO move to a clam lib

class BufferOscilloscopeMonitor :
	public CLAM::PortMonitor<CLAM::Audio>,
	public CLAM::VM::FloatArrayDataSource
{
public:
	BufferOscilloscopeMonitor()
		: _size(0)
		{ }
private:
	const char* GetClassName() const { return "BufferOscilloscope"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string a;
		return a;
	}
	const CLAM::TData * frameData()
	{
		const CLAM::Audio & audio = FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetBuffer();
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
	bool hasUpperBound() const { return true; }
	bool hasLowerBound() const { return true; }
	CLAM::TData upperBound() const {return 1;}
	CLAM::TData lowerBound() const {return -1;}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
	
};


#endif//BufferOscilloscopeMonitor_hxx
