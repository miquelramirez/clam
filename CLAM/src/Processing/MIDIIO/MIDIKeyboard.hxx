#ifndef _MIDIKeyboard_HXX_
#define _MIDIKeyboard_HXX_

#include "OutControl.hxx"
#include "OutControlPublisher.hxx"
#include "InControl.hxx"
#include "MIDIInControl.hxx"
#include "MIDIClocker.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM 
{

class MIDIKeyboardConfig : public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (MIDIKeyboardConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, MidiDevice);

	void DefaultInit(void);
};

/* simple wrapper to provide quick and dirty access to MIDI devices in NetworkEditor */
class MIDIKeyboard : public Processing
{
		MIDIKeyboardConfig mConfig;

		TControlData mCurrentTime;
		TControlData mCurrentTimeIncrement;

		MIDIIOConfig mNoteInConfig;
		MIDIInControl mNoteIn;

		FloatInControl mNoteInControl;
		FloatInControl mVelocityInControl;
		FloatInControl mPitchBendInControl;
		FloatInControl mModulationInControl;

		MIDIIOConfig mPitchBendInConfig;
		MIDIInControl mPitchBendIn;

		MIDIIOConfig mModulationConfig;
		MIDIInControl mModulationIn;

		MIDIClockerConfig mClockerConfig;
		MIDIClocker mClocker;

		OutControlPublisher mNoteOut;
		OutControlPublisher mVelocityOut;
		OutControlPublisher mPitchBendOut;
		OutControlPublisher mModulationOut;

	public:

		const char* GetClassName() const { return "MIDIKeyboard"; }

		MIDIKeyboard();
		
		MIDIKeyboard( ProcessingConfig& cfg );

		bool ConcreteConfigure( const ProcessingConfig& cfg );

		bool Do();

		~MIDIKeyboard() {}

		const ProcessingConfig& GetConfig() const { return mConfig; }

	private:

		bool ConfigureAndCheck( Processing& p, ProcessingConfig& cfg );
};

} // namespace CLAM 


#endif

