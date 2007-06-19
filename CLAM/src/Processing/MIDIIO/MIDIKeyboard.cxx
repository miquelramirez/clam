#include "AudioManager.hxx"
#include "MIDIManager.hxx"

#include "ProcessingFactory.hxx"
#include "MIDIKeyboard.hxx"


namespace CLAM 
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, MIDIKeyboard> regMIDIKeyboard("MIDIKeyboard");
}

void MIDIKeyboardConfig::DefaultInit(void)
{
	AddMidiDevice();
	UpdateData();

	SetMidiDevice("alsa:hw:1,0");
}

MIDIKeyboard::MIDIKeyboard()
	:
	mCurrentTime( 0.0 ),
	mCurrentTimeIncrement( 0 ),	
	mNoteInControl( "NoteIn", this, false ),
	mVelocityInControl( "VelocityIn", this, false ),
	mPitchBendInControl( "PitchBendIn", this, false ),
	mModulationInControl( "ModulationIn", this, false ),
	mNoteOut( "NoteOut", this ),
	mVelocityOut( "VelocityOut", this ),
	mPitchBendOut( "PitchBendOut", this ),
	mModulationOut( "ModulationOut", this )
{ 
	Configure( mConfig );
}

MIDIKeyboard::MIDIKeyboard( ProcessingConfig& cfg )
	:
	mCurrentTime( 0.0 ),
	mCurrentTimeIncrement( 0 ),	
	mNoteInControl( "NoteIn", this),
	mVelocityInControl( "VelocityIn", this, false ),
	mPitchBendInControl( "PitchBendIn", this, false ),
	mModulationInControl( "ModulationIn", this, false ),
	mNoteOut( "NoteOut", this ),
	mVelocityOut( "VelocityOut", this ),
	mPitchBendOut( "PitchBendOut", this ),
	mModulationOut( "ModulationOut", this )
{
	Configure( cfg );	
}

bool MIDIKeyboard::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );

	mConfig.SetMidiDevice( "alsa:hw:1,0" );

//	mConfig.SetMidiDevice( "file:test.mid" );

	mNoteInConfig.SetDevice( mConfig.GetMidiDevice() );
	mNoteInConfig.SetMessage( MIDI::eNoteOnOff );
	ConfigureAndCheck( mNoteIn, mNoteInConfig );

	mPitchBendInConfig.SetDevice( mConfig.GetMidiDevice() );
	mPitchBendInConfig.SetMessage( MIDI::ePitchbend );
	ConfigureAndCheck( mPitchBendIn, mPitchBendInConfig );

	mModulationConfig.SetDevice( mConfig.GetMidiDevice() );
	mModulationConfig.SetMessage(MIDI::eControlChange);
	mModulationConfig.SetFirstData( 0x01 );	// modulation 
	ConfigureAndCheck( mModulationIn, mModulationConfig );

	OutControl& outNote = mNoteIn.GetOutControls().GetByNumber(1);
	OutControl& outVelocity = mNoteIn.GetOutControls().GetByNumber(2); 
	OutControl& outPitchBend = mPitchBendIn.GetOutControls().GetByNumber(1);
	OutControl& outModulation = mModulationIn.GetOutControls().GetByNumber(1);

	mNoteOut.PublishOutControl( outNote );
	mVelocityOut.PublishOutControl( outVelocity );
	mPitchBendOut.PublishOutControl( outPitchBend );
	mModulationOut.PublishOutControl( outModulation );	

	mNoteOut.AddLink( mNoteInControl );
	mVelocityOut.AddLink( mVelocityInControl );
	mPitchBendOut.AddLink( mPitchBendInControl );
	mModulationOut.AddLink( mModulationInControl );

	mClockerConfig.SetDevice( mConfig.GetMidiDevice() );
	mClocker.Configure( mClockerConfig );

	return true;
}

bool MIDIKeyboard::Do()
{
	TData buffersize = 512.0;	

	mCurrentTimeIncrement = buffersize * 1000.0 / CLAM::AudioManager::Current().SampleRate();

	mClocker.GetInControls().GetByNumber(0).DoControl( mCurrentTime );

	mNoteOut.SendControl( mNoteInControl.GetLastValue() );
	mVelocityOut.SendControl( mVelocityInControl.GetLastValue() );
	mPitchBendOut.SendControl( mPitchBendInControl.GetLastValue() );
	mModulationOut.SendControl( mModulationInControl.GetLastValue() );

	mCurrentTime += mCurrentTimeIncrement;

	CLAM::MIDIManager::Current().Check();

	return true;
}

bool MIDIKeyboard::ConfigureAndCheck( Processing& p, ProcessingConfig& cfg )
{
	bool configurationOk = p.Configure(cfg);
	CLAM_ASSERT( configurationOk, p.GetConfigErrorMessage().c_str() );
	
	return configurationOk; 
}


} // namespace CLAM 

