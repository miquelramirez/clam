#ifndef _CSaltoMIDIInOut_
#define _CSaltoMIDIInOut_

#include "CMTGMIDIIn.hxx"
#include "CMTGMIDITarget.hxx"
//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"
#include "CSaltoMIDITarget.hxx"


class CSaltoMIDIInOut
{
public:

//CSaltoMIDIInOut(CSaltoParameter* pParams);
CSaltoMIDIInOut(MTG::Parameters* pParams);
~CSaltoMIDIInOut();

void CheckMIDI();

void SimulateMidiProcess1();
void SimulateMidiProcess2();

private:

 CMTGMIDICard         mCard;   // default 0 for OMS
 CMTGMIDIDevice       mDevice; // default takes the first device in the OMS Setup
 CMTGMIDISettings     mMidiSettings;  
 CSaltoMIDITarget     mSaltoMIDITarget;
 CMTGMIDIIn           mMidiIn;
 TMIDITime            mTimestamp;
 
 int                  mLastPitch;
 short                mControl1,mControl2,mControl3,mControl4;

 // Timbre Handling 
 CSaltoTimbreVektor   mTimbreVektor;
 
 unsigned char	      mMidiData[4];
};
#endif

