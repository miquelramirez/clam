#ifndef VumeterRMSPrinter_hxx
#define VumeterRMSPrinter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <fstream>
#include <cmath>

namespace CLAM
{

class VumeterRMSPrinter : public CLAM::Processing
{
	AudioInPort _input;
	double _energy;
	double _lastLogEnergy;
	unsigned _sampleCount;

public:
	VumeterRMSPrinter(const Config& config = Config())
		: _input("Input", this)
		, _energy(0.)
		,_lastLogEnergy(-50) 
		, _sampleCount(0)
	{
		Configure( config );
	}
	

	bool Do()
	{
		const int sampleRate = 44100;
		const int samplesPerFrame = sampleRate/25;
		unsigned size = _input.GetAudio().GetBuffer().Size();
		const CLAM::TData * data = &(_input.GetAudio().GetBuffer()[0]);
		
		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & current = data[i];
			_sampleCount++;
			_energy += current*current/size;
			if (_sampleCount >= samplesPerFrame)
			{
				double logEnergy =10 + 10*log(_energy);
				
				//if (logEnergy < -60)
				//	logEnergy = -60;
				//if (logEnergy > 0)
				//	logEnergy = 0.;
				//if(logEnergy-_lastLogEnergy<-5) 
					//logEnergy=_lastLogEnergy-5;

				std::ofstream file("vumeterRMS.data", std::fstream::app);
				file << logEnergy << std::endl;
				_energy = 0.;
				_sampleCount=0;
				_lastLogEnergy = logEnergy;
			}
		}

		_input.Consume();
		return true;
	}
	const char* GetClassName() const
	{
		return "VumeterRMSPrinter";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		std::ofstream file("vumeterRMS.data"); // erase the file if exists
		return true;
	}
};

} //namespace

#endif
