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

public:
	VumeterPrinter(const Config& config = Config())
		: _input("Input", this)
	{
		Configure( config );
	}
	

	bool Do()
	{

		double max = 1e-10;
		unsigned size = _input.GetAudio().GetBuffer().Size();
		const CLAM::TData * data = &(_input.GetAudio().GetBuffer()[0]);

		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & bin = data[i];
			if (bin>max) max=bin;
			if (bin<-max) max=-bin;
		}
		double logEnergy = 60 + 20*log(max);

		std::ofstream file("vumeter.data", std::fstream::app);
		file << logEnergy << std::endl;

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
