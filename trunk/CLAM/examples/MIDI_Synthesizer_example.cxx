/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
	:	AudioApplication(),
		mMidiDeviceStr(midiDeviceStr),
		mAudioDeviceStr(audioDeviceStr)
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

void MyAudioApplication::ConfigureAndCheck(Processing& p,ProcessingConfig& cfg)
{
	CLAM_ASSERT(p.Configure(cfg),p.GetConfigErrorMessage().c_str());
}

void MyAudioApplication::AudioMain(void)
{
	TControlData curTime = 0.;
	TControlData curTimeInc = 0.;
	try
	{
		const int nVoices = 6;
		// TODO: this is a bit of a kludge
		// to make sure all buffersizes us the default AudioOutPort buffersize
		AudioOutPort dummy("dummy",0);
		unsigned int buffersize = dummy.GetSize(); 

		// Audio and MIDI managers
		AudioManager audioManager(44100,4096);
		MIDIManager midiManager;

		// AudioIn
		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;

		inCfgL.SetDevice(mAudioDeviceStr);
		inCfgL.SetChannelID(0);
		inCfgL.SetFrameSize(buffersize);
		
		inCfgR.SetDevice(mAudioDeviceStr);
		inCfgR.SetChannelID(1);
		inCfgR.SetFrameSize(buffersize);
	
		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);

		// AudioOut
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;

		outCfgL.SetDevice(mAudioDeviceStr);
		outCfgL.SetChannelID(0);
		outCfgL.SetFrameSize(buffersize);

		outCfgR.SetDevice(mAudioDeviceStr);
		outCfgR.SetChannelID(1);
		outCfgR.SetFrameSize(buffersize);

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
		for (int i=0;i<nVoices;i++)
		{
			char tmp[10];
			sprintf(tmp,"instrument%d",i);
			instrumentCfg[i].SetAttackTime( (TData) 0.05 );
			instrumentCfg[i].SetDecayTime( (TData) 0.07 );
			instrumentCfg[i].SetSustainLevel( (TData) 0.5 );
			instrumentCfg[i].SetReleaseTime( (TData) 0.05 );
		}

		Array< Instrument* > instruments( nVoices );

		for (int i=0;i<nVoices;i++)
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

		for( int i = 0; i < nVoices; i++ )
		{
			audioArray.AddElem( bufOsc );
		}

		// Output buffer
		Audio out;
		out.SetSize( buffersize );

		// Mixer Declaration
		AudioMixerConfig mixerCfg;
		mixerCfg.SetFrameSize(buffersize);
		mixerCfg.SetNumberOfInPorts(nVoices);

		AudioMixer mixer;
		mixer.Configure(mixerCfg);

		/* Connecting audio inputs / outputs
		 * TODO port-names should be homogenic! */
		for ( int i=0;i<nVoices;i++)
		{
			std::ostringstream sstr;
			sstr << "Input " << i;
			ConnectPorts(*instruments[i],"AudioOut", mixer, sstr.str());
		}
		ConnectPorts(mixer,"Output Audio", outR, "Audio Input");
		ConnectPorts(mixer,"Output Audio", outL, "Audio Input");
		
		/** Ignoring channel, which is OutControl 0 */
		
		/** Key for Note On/Off */
		ConnectControls(inNote, 1, dispatcher, 1);
		/** Velocity for Note On/Off */
		ConnectControls(inNote, 2, dispatcher, 2);
		
		for( int i = 0; i < nVoices; i++ )
			ConnectControls(inPitchBend, 1, *instruments[i], 3);
		
		mixer.Start();

		inL.Start();
		inR.Start();
		outL.Start();
		outR.Start();

		midiManager.Start();

		audioManager.Start();

		for ( int i = 0; i < nVoices; i++ )
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
		
			for ( int i = 0; i < nVoices; i++ )
			{
				instruments[ i ]->Do();
			}

			mixer.Do();

			outL.Do();
			outR.Do();
		} while (!Canceled()) ;

		for ( int i = 0; i < nVoices; i++ )
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
	//const char* midiDeviceStr = "alsa:hw:1,0";
	const char* midiDeviceStr = "file:test.mid";
	const char* audioDeviceStr = "default";
	
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

