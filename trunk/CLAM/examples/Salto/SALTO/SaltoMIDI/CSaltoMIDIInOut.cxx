#include "CSaltoMIDIInOut.hxx"
#include <stdio.h>

//CSaltoMIDIInOut::CSaltoMIDIInOut(CSaltoParameter* pParams)
CSaltoMIDIInOut::CSaltoMIDIInOut(MTG::Parameters* pParams)
:mDevice(MIDI_DEVICE_NR),mMidiSettings(mCard, mDevice, false),mSaltoMIDITarget(pParams),mMidiIn(mMidiSettings,mSaltoMIDITarget)
{
}

CSaltoMIDIInOut::~CSaltoMIDIInOut()
{
}
//----------------------------------------------------------------------------//
void
CSaltoMIDIInOut::CheckMIDI()
{
#if defined LINUX || defined WIN32
	  std::cout << "CSaltoMIDIInOut::CheckMIDI()" << endl;
#endif // LINUX

  mMidiIn.Read();
}

//----------------------------------------------------------------------------//
/* as we cannot debug midi with the activated OMS Callback Hook, this is a funtion
 which could be called from the gui editor to simulate diverse MIDI events . With
 this function its possible by unplugging the MIDI Interface, to start up the App
 without enabling the callback hook ! */

void CSaltoMIDIInOut::SimulateMidiProcess1()
{
  mMidiData[0] = 176; // use higher number of Transition Frames 
  mMidiData[1] = 65;
  mMidiData[2] = 10;  
  mSaltoMIDITarget.Handle(0,mMidiData,3);
  
  mMidiData[0] = 144; // Note on
  mMidiData[1] = 65;   // c3
  mMidiData[2] = 110;  
  mSaltoMIDITarget.Handle(0,mMidiData,3);

  mMidiData[0] = 176; // Breath velocity
  mMidiData[1] = 60;
  mMidiData[2] = 110;  
  mSaltoMIDITarget.Handle(0,mMidiData,3);
}

void CSaltoMIDIInOut::SimulateMidiProcess2()
{ 
  mMidiData[0] = 144; // Note on new pitch
  mMidiData[1] = 55;
  mMidiData[2] = 100;   
  mSaltoMIDITarget.Handle(0,mMidiData,3);
}
