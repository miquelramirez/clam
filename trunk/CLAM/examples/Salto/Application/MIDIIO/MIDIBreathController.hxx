#ifndef __MIDIBREATHCONTROLLER__
#define __MIDIBREATHCONTROLLER__

#include "MIDIInControl.hxx"

namespace SALTO
{

using CLAM::MIDIInControl;
using CLAM::MIDIInConfig;

struct BreathController
{

	MIDIInControl	mInNote;
	MIDIInControl	mAirSpeed;
	
	BreathController( const MIDIInConfig &note,
					  const MIDIInConfig &airspeed )
		: mInNote( note ), mAirSpeed( airspeed ) 
	{
	}
	
	BreathController()
	{
	}
};

}

#endif // MIDIBreathController.hxx
