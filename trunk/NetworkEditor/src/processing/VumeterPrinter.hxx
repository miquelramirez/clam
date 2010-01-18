#ifndef VumeterPrinter_hxx
#define VumeterPrinter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <fstream>
#include <cmath>

namespace CLAM
{

class VumeterPrinter : public CLAM::Processing
{
	AudioInPort _input;
	double _max;
	unsigned _sampleCount;

public:
	VumeterPrinter(const Config& config = Config())
		: _input("Input", this)
		, _max(0.)
		, _sampleCount(0)
	{
		Configure( config );
	}
	

	bool Do()
	{
		const unsigned sampleRate = 44100;
		const unsigned samplesPerFrame = sampleRate/25;
		unsigned size = _input.GetAudio().GetBuffer().Size();
		const CLAM::TData * data = &(_input.GetAudio().GetBuffer()[0]);

		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & current = data[i];
			if (current>_max) _max=current;
			if (current<-_max) _max=-current;
			_sampleCount++;
			if (_sampleCount >= samplesPerFrame)
			{
				double logEnergy = 60 + 20*log(_max);
				std::ofstream file("vumeter.data", std::fstream::app);
				file << logEnergy << std::endl;
				_max = 1e-10;
				_sampleCount=0;
			}
		}

		_input.Consume();
		return true;
	}
	const char* GetClassName() const
	{
		return "VumeterPrinter";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		std::ofstream file("vumeter.data"); // erase the file if exists
		return true;
	}
};

} //namespace

#endif
