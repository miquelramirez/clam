/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


/*
A FLOW DIAGRAM TO DEMONSTRATE THIS EXAMPLE CAN BE FOUND IN
CLAM-Docs/MIDI_Synthesizer_example (development-branch)
*/

#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "Oscillator.hxx"
#include "AudioApplication.hxx"
#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIClocker.hxx"
#include "Dispatcher.hxx"
#include "AudioMixer.hxx"
#include "AudioManager.hxx"
#include "TopLevelProcessing.hxx"
#include <vector>
#include <iostream>

using namespace CLAM;

class MyAudioApplication:public AudioApplication
{
	void AppCleanup() {}
private:
	const char* mMidiDeviceStr;
	const char* mAudioDeviceStr;
	void ConfigureAndCheck(Processing& p,ProcessingConfig& cfg);
public:
	void AudioMain(void);	
	MyAudioApplication(const char* midiDeviceStr,const char* audioDeviceStr)
	:	mMidiDeviceStr(midiDeviceStr),
		mAudioDeviceStr(audioDeviceStr),
		AudioApplication()
	{
	}
};

class MyInstrumentConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MyInstrumentConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, TData, AttackTime);
	DYN_ATTRIBUTE (2, public, TData, DecayTime);
	DYN_ATTRIBUTE (3, public, TData, SustainLevel);
	DYN_ATTRIBUTE (4, public, TData , ReleaseTime);
	DYN_ATTRIBUTE (5, public, TData , SamplingRate);
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
			mADSR.GetOutControls().GetByNumber(0).AddLink(&GetInControls().GetByNumber(0));

			GetOutControls().GetByNumber(1).AddLink(&mMapperNote.GetInControls().GetByNumber(0));
			GetOutControls().GetByNumber(2).AddLink(&mMapperVel.GetInControls().GetByNumber(0));
			GetOutControls().GetByNumber(3).AddLink(&mMapperPitchBend.GetInControls().GetByNumber(0));

			mMapperNote.GetOutControls().GetByNumber(0).AddLink(&mFreqMultiplier.GetInControls().GetByNumber(0));
			mMapperPitchBend.GetOutControls().GetByNumber(0).AddLink(&mFreqMultiplier.GetInControls().GetByNumber(1));

			mFreqMultiplier.GetOutControls().GetByNumber(0).AddLink(&mOscillator.GetInControls().GetByNumber(0));
			mMapperVel.GetOutControls().GetByNumber(0).AddLink(&mADSR.GetInControls().GetByNumber(0));
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

		bool Do(void) { return true; }

		bool Do(Audio& out) ;
};


void MyInstrumentConfig::DefaultInit(void)
{
	AddName();
	AddAttackTime();
	AddDecayTime(),
	AddSustainLevel();
	AddReleaseTime();
	AddSamplingRate();

	UpdateData();

	try
	{
		SetSamplingRate( AudioManager::Current().SampleRate() );
	}
	catch(Err)
	{
		SetSamplingRate( 8000 );
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
	ADSRCfg.SetSamplingRate( mConfig.GetSamplingRate() );

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

void MyAudioApplication::ConfigureAndCheck(Processing& p,ProcessingConfig& cfg)
{
	CLAM_ASSERT(p.Configure(cfg),p.GetStatus().c_str());
}

void MyAudioApplication::AudioMain(void)
{
	TControlData curTime = 0.;
	TControlData curTimeInc = 0.;
	try
	{
		const int nVoices = 4;
		unsigned int buffersize = 256;

		// Audio and MIDI managers
		AudioManager audioManager(44100,4096);
		MIDIManager midiManager;

		// AudioIn
		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;

		inCfgL.SetDevice(mAudioDeviceStr);
		inCfgL.SetChannelID(0);

		inCfgR.SetDevice(mAudioDeviceStr);
		inCfgR.SetChannelID(1);

	
		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);

		// AudioOut
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;

		outCfgL.SetDevice(mAudioDeviceStr);
		outCfgL.SetChannelID(0);

		outCfgR.SetDevice(mAudioDeviceStr);
		outCfgR.SetChannelID(1);

		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);

		// Buffers
		Audio bufL;
		bufL.SetSize(buffersize);
		Audio bufR;
		bufR.SetSize(buffersize);

		// If we pass a non-existant device, an error gets thrown somewhere
		// deep done, which gets cought in Configure, and added to the Status.
		// The ConfigureAndCheck will assert on this.
		
		// MIDIInControls
		MIDIIOConfig inNoteCfg;
		
		inNoteCfg.SetDevice(mMidiDeviceStr);
		inNoteCfg.SetMessage(MIDI::eNoteOnOff);
		
		MIDIInControl inNote;
		ConfigureAndCheck(inNote,inNoteCfg);

		MIDIIOConfig inCtrlCfg;
		
		inCtrlCfg.SetDevice(mMidiDeviceStr);
		inCtrlCfg.SetMessage(MIDI::eControlChange);
		inCtrlCfg.SetFirstData(0x0a);
		
		MIDIInControl inCtrl;
		ConfigureAndCheck(inCtrl,inCtrlCfg);

		MIDIIOConfig inPitchBendCfg;
		
		inPitchBendCfg.SetDevice(mMidiDeviceStr);
		inPitchBendCfg.SetMessage(MIDI::ePitchbend);
		
		MIDIInControl inPitchBend;
		ConfigureAndCheck(inPitchBend,inPitchBendCfg);

		MIDIClockerConfig clockerCfg;

		clockerCfg.SetDevice(mMidiDeviceStr);
		
		MIDIClocker clocker(clockerCfg);

		// Instrument
		MyInstrumentConfig instrumentCfg[ nVoices ];
		int i;
		for (i=0;i<nVoices;i++)
		{
			char tmp[10];
			sprintf(tmp,"instrument%d",i);
			instrumentCfg[i].SetAttackTime( (TData) 0.05 );
			instrumentCfg[i].SetDecayTime( (TData) 0.07 );
			instrumentCfg[i].SetSustainLevel( (TData) 0.5 );
			instrumentCfg[i].SetReleaseTime( (TData) 0.05 );
		}

		Array< Instrument* > instruments( nVoices );

		for (i=0;i<nVoices;i++)
		{
			instruments.AddElem( new MyInstrument( instrumentCfg[i] ) );
		}

		// Dispatcher declaration
		DispatcherConfig dispatcherCfg;
		
		dispatcherCfg.SetInstruments( instruments );
		dispatcherCfg.SetNInValues( 2 ) ;

		Dispatcher dispatcher( dispatcherCfg );

		// Audio Array
		Array< Audio > audioArray( nVoices );

		Audio bufOsc;
		bufOsc.SetSize(buffersize);

		for( i = 0; i < nVoices; i++ )
		{
			audioArray.AddElem( bufOsc );
		}

		// Output buffer
		Audio out;
		out.SetSize( buffersize );

		// Mixer Declaration
		AudioMixerConfig mixerCfg;
		mixerCfg.SetFrameSize(buffersize);
//		mixerCfg.SetSampleRate(audioManager.SampleRate());

		AudioMixer<nVoices> mixer;
		mixer.Configure(mixerCfg);

		for ( i=0;i<nVoices;i++)
		{
			std::stringstream sstr;
			sstr.str("");
			std::string name("Input Audio");
			sstr << name << "_" << i;
			mixer.GetInPorts().Get(sstr.str()).Attach(audioArray[i]);
		}
		mixer.GetOutPorts().Get("Output Audio").Attach(out);

		/** Ignoring channel, which is OutControl 0 */
		
		/** Key for Note On/Off */
		inNote.GetOutControls().GetByNumber(1).AddLink(&dispatcher.GetInControls().GetByNumber(1));
		/** Velocity for Note On/Off */
		inNote.GetOutControls().GetByNumber(2).AddLink(&dispatcher.GetInControls().GetByNumber(2));
		
		for( i = 0; i < nVoices; i++ )
			inPitchBend.GetOutControls().GetByNumber(1).AddLink(&instruments[i]->GetInControls().GetByNumber(3));
		
		mixer.Start();

		inL.Start();
		inR.Start();
		outL.Start();
		outR.Start();

		midiManager.Start();

		audioManager.Start();

		for ( i = 0; i < nVoices; i++ )
		{
			instruments[ i ]->Start();
		}

		curTimeInc = TData(buffersize)*1000./audioManager.SampleRate();

		do
		{
			inL.Do(bufL);
			inR.Do(bufR);

			clocker.GetInControls().GetByNumber(0).DoControl(curTime);
			curTime += curTimeInc;
			
			midiManager.Check();
			
			for ( i = 0; i < nVoices; i++ )
			{
				instruments[ i ]->Do( audioArray[ i ] );
			}

			mixer.Do();

			outL.Do( out );
			outR.Do( out );

		} while (!Canceled()) ;

		for ( i = 0; i < nVoices; i++ )
			delete instruments[ i ];

	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		exit(-1);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		exit(-1);
	}
}

int main(int argc,char** argv)
{
	char* midiDeviceStr = "alsa:hw:1,0"; // TODO: but back to default
	char* audioDeviceStr = "default";
	
	try
	{
		MyAudioApplication app(midiDeviceStr,audioDeviceStr);
		app.Run(argc,argv);
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		return -1;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}

