#ifndef __MIDIBREATHCONTROLLER__
#define __MIDIBREATHCONTROLLER__

#include "MIDIInControl.hxx"

namespace SALTO
{

struct BreathController
{

	CLAM::MIDIInControl	mInNote;
	CLAM::MIDIInControl	mAirSpeed;
	
	BreathController( const CLAM::MIDIIOConfig &note, const CLAM::MIDIIOConfig &airspeed )
		: mInNote( note ), mAirSpeed( airspeed ) 
	{
	}
	
	BreathController()
	{
	}
};

}

#endif // MIDIBreathController.hxx
