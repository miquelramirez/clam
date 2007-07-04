#ifndef ConstantSpectrum_hxx
#define ConstantSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{

class ConstantSpectrum : public Processing
{ 
	OutPort<MagPhaseSpectrum> mOut;
public:
	const char* GetClassName() const { return "ConstantSpectrum"; }
	ConstantSpectrum(const Config& config = Config()) 
		: mOut("My Output", this) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		MagPhaseSpectrum & spectrum = mOut.GetData();
		const unsigned size = 513;
		if (!spectrum.magnitudes.size()) 
		{
				spectrum.spectralRange=22050;
				spectrum.magnitudes.resize(size);
				spectrum.phases.resize(size);
				for (unsigned i = 0; i<size; i++)
					spectrum.magnitudes[i]=.1+float(i)/size;
		}

		// Tell the ports this is done
		mOut.Produce();
		return true;
	}

};

}

#endif // ConstantSpectrum_hxx
