
#ifndef __Oscillator_hxx__
#define __Oscillator_hxx__ 

#include "SimpleOscillator.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Audio.hxx"
#include "InPortTmpl.hxx"
#include "InControl.hxx"
#include <string>

namespace CLAM
{
	
class OscillatorConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OscillatorConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData, Amplitude);
	DYN_ATTRIBUTE (2, public, TData, ModIndex);
	DYN_ATTRIBUTE (3, public, TData , Phase);
	DYN_ATTRIBUTE (4, public, TData , SamplingRate);
protected:
	void DefaultInit(void);
};

class Oscillator : public SimpleOscillator
{
	OscillatorConfig mConfig;

	InPortTmpl<Audio> mInputPhaseMod;
	InPortTmpl<Audio> mInputFreqMod;
	TData mModIndex;

	typedef InControlTmpl<Oscillator> OscillatorCtrl;

	Oscillator::OscillatorCtrl * mModIdxCtl;

	bool           mModIdxUpdated;
	
	inline void ApplyControls()
	{
		ApplyFreqAndAmpControls();
		if ( mModIdxUpdated )
			{
				mModIndex = mModIdxCtl->GetLastValue();
				mModIdxUpdated = false;
			}
	}
	int UpdateModIdx( TControlData );

public:
	Oscillator();
	
	Oscillator(const OscillatorConfig& c );

	virtual ~Oscillator();
	bool ConcreteConfigure( const ProcessingConfig& c );

	const char * GetClassName() const {return "Oscillator";}
	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do();
	bool Do(Audio & out)
	{
		return SimpleOscillator::Do(out);
	}
	bool Do(const Audio& pitchModIn,const Audio& phaseModIn,Audio& out);
	bool Do(const Audio& pitchModIn,const int& dum ,Audio& out);
	bool Do(const int& dum ,const Audio& phaseModIn,Audio& out);
};

}

#endif //__Oscillator_hxx__ 
