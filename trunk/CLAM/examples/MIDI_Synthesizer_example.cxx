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
#include "Dispatcher.hxx"
#include "Mixer.hxx"
#include "AudioManager.hxx"
#include <vector>
#include <iostream>

using namespace CLAM;

class MyAudioApplication:public AudioApplication
{
	void AppCleanup() {}
public:
	void AudioMain(void);	
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
			mADSR.LinkOutWithInControl( 0, this, 0 );

			LinkOutWithInControl( 1, &mMapperNote, 0 );
			LinkOutWithInControl( 2, &mMapperVel, 0 );
			LinkOutWithInControl( 3, &mMapperPitchBend, 0 );	

			mMapperNote.LinkOutWithInControl( 0, &mFreqMultiplier, 0 );
			mMapperPitchBend.LinkOutWithInControl( 0, &mFreqMultiplier, 1 );

			mFreqMultiplier.LinkOutWithInControl( 0, &mOscillator, 0 );
			mMapperVel.LinkOutWithInControl( 0, &mADSR, 0 );
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

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);

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
		SetSamplingRate( AudioManager::Singleton().SampleRate() );
	}
	catch(Err)
	{
		SetSamplingRate( 8000 );
	}

}


bool MyInstrument::ConcreteConfigure( const ProcessingConfig& cfg) throw( std::bad_cast )
{
	mConfig = dynamic_cast< const MyInstrumentConfig& >(cfg);

	ADSRConfig ADSRCfg;
	std::string tmp = mConfig.GetName() + ".ADSR";

	ADSRCfg.SetName( tmp );
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

bool MyInstrument::Do( Audio& out )
{
	mEnvelope.SetSize( out.GetSize() );

	mOscillator.Do( out );
	mADSR.Do( mEnvelope );
	mSampleMultiplier.Do( out, mEnvelope, out );

	return true;
}


void MyAudioApplication::AudioMain(void)
{
	try
	{

		unsigned int buffersize = 512;

		// Audio and MIDI managers
		AudioManager audioManager(48000,512);
		MIDIManager midiManager;

		// AudioIn
		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;

		inCfgL.SetName("left in");
		inCfgL.SetChannelID(0);

		inCfgL.SetName("right in");
		inCfgL.SetChannelID(1);

	
		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);



		// AudioOut
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;

		outCfgL.SetName("left out");
		outCfgL.SetChannelID(0);

		outCfgL.SetName("right out");
		outCfgL.SetChannelID(1);

		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);

		// Buffers
		Audio bufL;
		bufL.SetSize(buffersize);
		Audio bufR;
		bufR.SetSize(buffersize);


		// MIDIInControls
		MIDIInConfig inNoteCfg;
		
		inNoteCfg.SetName("in");
		inNoteCfg.SetDevice("default:default");
		inNoteCfg.SetChannelMask(
			MIDI::ChannelMask(1)|
			MIDI::ChannelMask(2)
		);

		inNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff)
		);
		
		MIDIInControl inNote(inNoteCfg);

		MIDIInConfig inCtrlCfg;
		
		inCtrlCfg.SetName("inctrl");
		inCtrlCfg.SetDevice("default:default");
		inCtrlCfg.SetChannelMask(MIDI::ChannelMask(1));
		inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
		inCtrlCfg.SetFilter(0x0a);
		
		MIDIInControl inCtrl(inCtrlCfg);

		MIDIInConfig inPitchBendCfg;
		
		inPitchBendCfg.SetName("inPitchBend");
		inPitchBendCfg.SetDevice("default:default");
		inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
		inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));
		
		MIDIInControl inPitchBend(inPitchBendCfg);

		// Instrument
		MyInstrumentConfig instrumentCfg[ 3 ];
		int i;
		for (i=0;i<3;i++)
		{
			char tmp[10];
			sprintf(tmp,"instrument%d",i);
			instrumentCfg[i].SetName(tmp);
			instrumentCfg[i].SetAttackTime( (TData) 0.2 );
			instrumentCfg[i].SetDecayTime( (TData) 0.1 );
			instrumentCfg[i].SetSustainLevel( (TData) 0.5 );
			instrumentCfg[i].SetReleaseTime( (TData) 0.5 );
		}

		Array< Instrument* > instruments( 3 );

		for (i=0;i<3;i++)
		{
			instruments.AddElem( new MyInstrument( instrumentCfg[i] ) );
		}

		// Dispatcher declaration
		DispatcherConfig dispatcherCfg;
		
		dispatcherCfg.SetName( "Dispatcher" );
		dispatcherCfg.SetInstruments( instruments );
		dispatcherCfg.SetNInValues( 2 ) ;

		Dispatcher dispatcher( dispatcherCfg );

		// Audio Array
		Array< Audio > audioArray( 3 );

		Audio bufOsc;
		bufOsc.SetSize(buffersize);

		for( i = 0; i < 3; i++ )
		{
			audioArray.AddElem( bufOsc );
		}

		// Output buffer
		Audio out;
		out.SetSize( buffersize );

		// Mixer Declaration
		Mixer mixer;

		inNote.LinkOutWithInControl( 0, &dispatcher, 1 );   /** Key for Note Off */
		inNote.LinkOutWithInControl( 1, &dispatcher, 2 );   /** Velocity for Note Off */
		inNote.LinkOutWithInControl( 2, &dispatcher, 1 );   /** Key for Note On */
		inNote.LinkOutWithInControl( 3, &dispatcher, 2 );   /** Velocity for Note On */

		inPitchBend.LinkOutWithInControl( 0, instruments[ 0 ] , 3 );
		inPitchBend.LinkOutWithInControl( 0, instruments[ 1 ] , 3 );
		inPitchBend.LinkOutWithInControl( 0, instruments[ 2 ] , 3 );

		midiManager.Start();

		audioManager.Start();

		do
		{
			inL.Do(bufL);
			inR.Do(bufR);

			midiManager.Check();

			for ( i = 0; i < 3; i++ )
			{
				instruments[ i ]->Do( audioArray[ i ] );
			}

			mixer.Do( audioArray, out );

			outL.Do( out );
			outR.Do( out );

		} while (!Canceled()) ;

		for ( i = 0; i < 3; i++ )
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
	try
	{
		MyAudioApplication app;
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

