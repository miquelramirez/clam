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

#include <CLAM/Dispatcher.hxx>
#include <CLAM/Oscillator.hxx>
#include <vector>
#include <iostream>
#include <unistd.h>

using namespace CLAM;

class MyInstrument:public Instrument
{
public:
		class Config: public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (Config, 6, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);
			DYN_ATTRIBUTE (1, public, TData, AttackTime);
			DYN_ATTRIBUTE (2, public, TData, DecayTime);
			DYN_ATTRIBUTE (3, public, TData, SustainLevel);
			DYN_ATTRIBUTE (4, public, TData , ReleaseTime);
			DYN_ATTRIBUTE (5, public, TData , SampleRate);
		protected:
			void DefaultInit()
			{
				AddAll();
				UpdateData();
				SetAttackTime(   0.05 );
				SetDecayTime(    0.07 );
				SetSustainLevel( 0.5 );
				SetReleaseTime(  0.05 );
				SetSampleRate( 0);
			}
		};

		Config            mConfig;
		ADSR                          mADSR;
		Audio                         mEnvelope;
		Oscillator                    mOscillator;
		AudioMultiplier               mSampleMultiplier;
		ControlMapper                 mMapperNote;
		ControlMapper                 mMapperVel;
		ControlMapper                 mMapperPitchBend;
		ControlMultiplier             mFreqMultiplier;
		FloatInControl                mPitchBendIn;
		FloatOutControl               mPitchBendOut;
	
		void UpdatePitchBend( TControlData value )
		{
			mPitchBendOut.SendControl( value );
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
		MyInstrument( const Config& c = Config())
			: mPitchBendIn( "PitchBend", this, &MyInstrument::UpdatePitchBend )
			, mPitchBendOut( "PitchBendOut", this )
		{
			  Configure( c );
		}

		~MyInstrument(){}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& c );
	
		bool ConcreteStart();

		bool Do(Audio& out) ;
};



bool MyInstrument::ConcreteConfigure( const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	unsigned sampleRate = mConfig.GetSampleRate();
	if (not sampleRate) sampleRate = BackendSampleRate();

	ADSRConfig ADSRCfg;
	std::string tmp = mConfig.GetName() + ".ADSR";

	ADSRCfg.SetAttackTime( mConfig.GetAttackTime() );
	ADSRCfg.SetDecayTime( mConfig.GetDecayTime() );
	ADSRCfg.SetSustainLevel( mConfig.GetSustainLevel() );
	ADSRCfg.SetReleaseTime( mConfig.GetReleaseTime() );
	ADSRCfg.SetSampleRate( sampleRate );

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

#include <CLAM/AudioMixer.hxx>
#include <CLAM/PANetworkPlayer.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>
#include <CLAM/MIDIManager.hxx>
#include <CLAM/MIDIInControl.hxx>
#include <CLAM/MIDIClocker.hxx>

std::string toString(unsigned i)
{
	std::ostringstream os;
	os << i;
	return os.str();
}

void runSynthesizer(const std::string midiDeviceStr)
{


	const int nVoices = 6;

	Network network;
	network.SetPlayer(new PANetworkPlayer);

	Processing & sink = network.AddProcessing("sink", "AudioSink");
	AudioSink::Config sinkConfig;
	sinkConfig.SetNSinks(2);
	sink.Configure(sinkConfig);

	//  MIDI manager
	MIDIManager midiManager;

	// If we pass a non-existant device, an error gets thrown somewhere
	// deep done, which gets cought in Configure, and added to the Status.
	// The ConfigureAndCheck will assert on this.
	
	// MIDIInControls
	MIDIIOConfig inNoteCfg;
	inNoteCfg.SetDevice(midiDeviceStr);
	inNoteCfg.SetMessage(MIDI::eNoteOnOff);
	network.AddProcessing("inNote", new MIDIInControl(inNoteCfg));

	MIDIIOConfig inCtrlCfg;
	inCtrlCfg.SetDevice(midiDeviceStr);
	inCtrlCfg.SetMessage(MIDI::eControlChange);
	inCtrlCfg.SetFirstData(0x0a);
	network.AddProcessing("inCtrl", new MIDIInControl(inCtrlCfg));

	MIDIIOConfig inPitchBendCfg;
	inPitchBendCfg.SetDevice(midiDeviceStr);
	inPitchBendCfg.SetMessage(MIDI::ePitchbend);
	network.AddProcessing("inPitchBend", new MIDIInControl(inPitchBendCfg));

	MIDIClocker::Config clockerCfg;
	clockerCfg.SetDevice(midiDeviceStr);
	network.AddProcessing("clocker", new MIDIClocker(clockerCfg));

	// Instrument
	Array< Instrument* > instruments( nVoices );
	for (int i=0;i<nVoices;i++)
	{
		instruments[i] = new MyInstrument;
		network.AddProcessing("instrument"+toString(i), instruments[i]);
	}
	// Dispatcher declaration
	DispatcherConfig dispatcherCfg;
	dispatcherCfg.SetInstruments( instruments );
	dispatcherCfg.SetNInValues( 2 ) ;
	network.AddProcessing("dispatcher", new Dispatcher(dispatcherCfg ));

	// Mixer Declaration
	AudioMixerConfig mixerCfg;
	mixerCfg.SetNumberOfInPorts(nVoices);
	network.AddProcessing("mixer", new AudioMixer, &mixerCfg);

	// TODO: Check configuration

	/* Connecting audio inputs / outputs
	 * TODO port-names should be homogenic! */
	for ( int i=0;i<nVoices;i++)
	{
		network.ConnectPorts("instrument"+toString(i)+".AudioOut", "mixer.Input "+toString(i));
	}
	network.ConnectPorts("mixer.Output Audio", "sink.1");
	network.ConnectPorts("mixer.Output Audio", "sink.2");
	network.ConnectPorts("mixer.Output Audio", "clocker.audioSync");
	
	/** Key for Note On/Off */
	network.ConnectControls("inNote.NoteOnOff:Key", "dispatcher.Note");
	/** Velocity for Note On/Off */
	network.ConnectControls("inNote.NoteOnOff:Vel", "dispatcher.Velocity");
	
	for( int i = 0; i < nVoices; i++ )
		network.ConnectControls("inPitchBend.Pitchbend:Value", "instrument"+toString(i)+".PitchBend");
	
	midiManager.Start();
	network.Start();

	do
	{
		usleep(10);
		midiManager.Check();
	} while (true) ;

}

int main(int argc,char** argv)
{
//	std::string midiDeviceStr = "alsa:hw:1,0";
	std::string midiDeviceStr = std::string("file:") + ( argc<2 ? "midi_out.mid": argv[1] );
	std::string audioDeviceStr = "default";
	
	try
	{
		runSynthesizer(midiDeviceStr);
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

