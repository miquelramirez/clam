#ifndef _MyInstrument_hxx_
#define _MyInstrument_hxx_

#include "ProcessingConfig.hxx"
#include "Instrument.hxx"
#include "AudioManager.hxx"

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

void MyInstrumentConfig::DefaultInit(void)
{
	AddName();
	AddAttackTime();
	AddDecayTime(),
	AddSustainLevel();
	AddReleaseTime();
	AddSampleRate();

	UpdateData();

	try
	{
		SetSampleRate( AudioManager::Current().SampleRate() );
	}
	catch(Err)
	{
		SetSampleRate( 8000 );
	}

}


bool MyInstrument::ConcreteConfigure( const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	ADSRConfig ADSRCfg;
	std::string tmp = mConfig.GetName() + ".ADSR";

	ADSRCfg.SetAttackTime( mConfig.GetAttackTime() );
	ADSRCfg.SetDecayTime( mConfig.GetDecayTime() );
	ADSRCfg.SetSustainLevel( mConfig.GetSustainLevel() );
	ADSRCfg.SetReleaseTime( mConfig.GetReleaseTime() );
	ADSRCfg.SetSampleRate( mConfig.GetSampleRate() );

	mADSR.Configure( ADSRCfg );

	ControlMapperConfig MapperVelCfg;

	MapperVelCfg.SetMapping( "linear" );
	TData ptr1[] = {0.0 ,127.0 ,0.0 ,1.0};
 	MapperVelCfg.SetArguments( DataArray( ptr1, 4 ) );
	mMapperVel.Configure( MapperVelCfg );

	ControlMapperConfig MapperNoteCfg;
	
	MapperNoteCfg.SetMapping( "NoteToFreq" );
	TData ptr2[] = {69.0,440.0} ;
	MapperNoteCfg.SetArguments( DataArray( ptr2, 2 ) );

	mMapperNote.Configure( MapperNoteCfg );


	ControlMapperConfig MapperPBendCfg;
	MapperPBendCfg.SetMapping( "ValueToRatio" );
	TData ptr3[] = { 12 } ;							
	MapperPBendCfg.SetArguments( DataArray( ptr3, 1 ) );

	mMapperPitchBend.Configure( MapperPBendCfg );

	LinkControls();

	return true;
}

bool MyInstrument::ConcreteStart()
{
	mOscillator.Start();
	mADSR.Start();
	mSampleMultiplier.Start();

	return true;
}

bool MyInstrument::Do( Audio& out )
{
	mEnvelope.SetSize( out.GetSize() );

	mOscillator.Do( out );
	mADSR.Do( mEnvelope );
	mSampleMultiplier.Do( out, mEnvelope, out );

	return true;
}




} //namespace
#endif
