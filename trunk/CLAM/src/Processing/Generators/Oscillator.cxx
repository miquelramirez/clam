
#include "Oscillator.hxx"

namespace CLAM
{
	
void OscillatorConfig::DefaultInit(void)
{
	AddFrequency();
	AddAmplitude();
	AddModIndex();
	AddPhase();
	AddSamplingRate();
	
	UpdateData();

	SetFrequency(440.0);
	SetAmplitude(1.0);
	SetModIndex(1.0);
	SetPhase(0.0);
	SetSamplingRate( 44100 );
}


Oscillator::Oscillator()
	:mInputPhaseMod("Input Phase Modulation", this, 1),
	mInputFreqMod("Input Frequency Modulation", this, 1),
	mModIdxUpdated( false ),
	mModIdxCtl(0)
{
	mModIdxCtl = new OscillatorCtrl( "ModIndex", this, &Oscillator::UpdateModIdx );
	
	OscillatorConfig cfg;
	Configure( cfg );
}

Oscillator::Oscillator(const OscillatorConfig& c )
	: mInputPhaseMod("Input Phase Modulation", this, 1),
	mInputFreqMod("Input Frequency Modulation", this, 1),
	mModIdxUpdated( false ),
	mModIdxCtl(0)
{	
	mModIdxCtl = new OscillatorCtrl( "ModIndex", this, &Oscillator::UpdateModIdx );

	SimpleOscillatorConfig simpleCfg;
	simpleCfg.SetFrequency( c.GetFrequency() );
	simpleCfg.SetAmplitude( c.GetAmplitude() );
	simpleCfg.SetSamplingRate( c.GetSamplingRate() );

	Configure( c );
}

Oscillator::~Oscillator()
{
	delete mModIdxCtl;
}

bool Oscillator::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);


	mAmp = mConfig.GetAmplitude();
	mPhase = mConfig.GetPhase(); // TEMP HACK  (See also constructor
	mModIndex = mConfig.GetModIndex();
	mSamplingRate = mConfig.GetSamplingRate();
	mDeltaPhase = TData(2.*PI*mConfig.GetFrequency()/mSamplingRate);

	return true;
}

bool Oscillator::Do()
{
	bool res =Do(mInputFreqMod.GetData(),mInputPhaseMod.GetData(),mOutput.GetData());			
	mInputFreqMod.LeaveData();
	mInputPhaseMod.LeaveData();
	mOutput.LeaveData();
	return res;
}

bool Oscillator::Do( const Audio& pitchModIn, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase + mModIndex*(*phaseModptr++)));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>2.*PI) 
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}

	return true;
}

bool Oscillator::Do( const Audio& pitchModIn, const int& dum, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	return true;
}

bool Oscillator::Do( const int& dum, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase + mModIndex*(*phaseModptr++)));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
			
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	
	return true;
}

int Oscillator::UpdateModIdx( TControlData value )
{
	mModIdxUpdated = true;

	return 0;
}


} // namespace CLAM

