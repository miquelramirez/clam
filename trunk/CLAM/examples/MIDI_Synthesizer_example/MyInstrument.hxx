#ifndef _MyInstrument_hxx_
#define _MyInstrument_hxx_

#include "Instrument.hxx"

namespace CLAM
{
	

class MyInstrumentConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyInstrumentConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, TData, AttackTime);
	DYN_ATTRIBUTE (2, public, TData, DecayTime);
	DYN_ATTRIBUTE (3, public, TData, SustainLevel);
	DYN_ATTRIBUTE (4, public, TData , ReleaseTime);
	DYN_ATTRIBUTE (5, public, TData , SampleRate);
protected:
	void DefaultInit(void);
};


class MyInstrument:public Instrument
{
public:
		MyInstrumentConfig            mConfig;
		ADSR                          mADSR;
		Audio                         mEnvelope;
		Oscillator                    mOscillator;
		AudioMultiplier               mSampleMultiplier;
		ControlMapper                 mMapperNote;
		ControlMapper                 mMapperVel;
		ControlMapper                 mMapperPitchBend;
		ControlMultiplier             mFreqMultiplier;
		InControlTmpl< MyInstrument >  mPitchBendIn;
		OutControl                    mPitchBendOut;
	
		int UpdatePitchBend( TControlData value )
		{
			mPitchBendOut.SendControl( value );

			return 0;
		}

		void LinkControls(void)
		{
			ConnectControls(mADSR, 0, *this, 0);

			ConnectControls(*this, 1, mMapperNote, 0);
			ConnectControls(*this, 2, mMapperVel, 0);
			ConnectControls(*this, 3, mMapperPitchBend, 0);

			ConnectControls(mMapperNote, 0, mFreqMultiplier, 0);
			ConnectControls(mMapperPitchBend, 0, mFreqMultiplier, 1);
			ConnectControls(mFreqMultiplier, 0, mOscillator, 0);
			ConnectControls(mMapperVel, 0, mADSR, 0);
		}

	public:
		MyInstrument():
		  mPitchBendIn( "PitchBend", this, &MyInstrument::UpdatePitchBend ),
		  mPitchBendOut( "PitchBendOut", this )
		{
			MyInstrumentConfig cfg;
			Configure( cfg );
		}

		MyInstrument( const MyInstrumentConfig& c):
		  mPitchBendIn( "PitchBend", this, &MyInstrument::UpdatePitchBend ),
		  mPitchBendOut( "PitchBendOut", this )
		{
			  Configure( c );
		}

		~MyInstrument(){}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& c );
	
		bool ConcreteStart();

		bool Do(Audio& out) ;
};

} //namespace
#endif
