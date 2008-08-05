/*
 * MIDIMelodyPlayer.cxx
 * Implementation for MIDIMelodyPlayer class
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/

#include <typeinfo>
#include <CLAM/xtime.hxx>
#include <CLAM/MIDIManager.hxx>
#include <CLAM/MIDIIOConfig.hxx>
#include <CLAM/MIDIOutControl.hxx>
#include "MIDIMelodyPlayer.hxx"

using namespace CLAM;

namespace V2M
{

MIDIMelodyPlayer::MIDIMelodyPlayer(const MIDIMelody& melody,
								   const std::string& device,
								   int program,
								   Slotv0& slot) 
								   : _melody(melody),
									_program(program)
{
	_device = "default:"+device;
	active = false;
	mRequestStop.Connect(slot);
	pthread_create(&mThread,NULL,sPlayMIDIThreadSafe,(void*)this);
}

MIDIMelodyPlayer::~MIDIMelodyPlayer()
{
	active = false;
	pthread_join(mThread,NULL);
}

void* MIDIMelodyPlayer::sPlayMIDIThreadSafe(void* ptr)
{
 	((MIDIMelodyPlayer*)ptr)->PlayMIDIThreadSafe();

	return NULL;
}

void MIDIMelodyPlayer::PlayMIDIThreadSafe()
{
	bool isBegin = true;
	int noteNumber = 0;
	int nNotes = _melody.GetNumberOfNotes();
	unsigned t1,t2;

	MIDIManager manager;

	// program
	MIDIIOConfig progChgCfg;
	progChgCfg.SetDevice(_device.c_str());
	progChgCfg.SetChannel(1);
	progChgCfg.SetMessage(MIDI::eProgramChange);
	MIDIOutControl progChg(progChgCfg);

	// volume
	MIDIIOConfig volCtrlCfg;
	volCtrlCfg.SetDevice(_device.c_str());
	volCtrlCfg.SetChannel(1);
	volCtrlCfg.SetFirstData(7); 
	volCtrlCfg.SetMessage(MIDI::eControlChange);
	MIDIOutControl volCtrl(volCtrlCfg);

	// note on
	MIDIIOConfig outNoteCfg;
	outNoteCfg.SetDevice(_device.c_str());
	outNoteCfg.SetChannel(1);
	outNoteCfg.SetMessage(MIDI::eNoteOn);
	MIDIOutControl outNote(outNoteCfg);

	manager.Start();

	SendFloatToInControl(progChg,0,TControlData(_program)); // program
	SendFloatToInControl(volCtrl,0,120); // volume = 120

	active = true;

	t1 = getmilliseconds();

    while(noteNumber < nNotes)
    {
		if(!active) break;

		t2 = getmilliseconds();
		if((t2-t1) >= _melody.GetNoteArray()[noteNumber].GetTime().GetBegin()*1000 && isBegin)
		{
			// note on
			SendFloatToInControl(outNote,0,TControlData(_melody.GetNoteArray()[noteNumber].GetKey()));
			SendFloatToInControl(outNote,1,TControlData(_melody.GetNoteArray()[noteNumber].GetVelocity()));
			isBegin = false;
		}
		if((t2-t1) >= _melody.GetNoteArray()[noteNumber].GetTime().GetEnd()*1000 && !isBegin)
		{
			// note off
			SendFloatToInControl(outNote,0,TControlData(_melody.GetNoteArray()[noteNumber].GetKey()));
			SendFloatToInControl(outNote,1,0);
			noteNumber++;
			isBegin = true;
		}
	}

	// send panic
	MIDIIOConfig panicCfg;
	panicCfg.SetDevice(_device.c_str());
	panicCfg.SetChannel(1);
	panicCfg.SetFirstData(123); 
	panicCfg.SetMessage(MIDI::eControlChange);
	MIDIOutControl panicCtrl(panicCfg);
	SendFloatToInControl(panicCtrl,0,0); 
    
	if(active)	mRequestStop.Emit();
}

void MIDIMelodyPlayer::Stop()
{
	active = false;
}

unsigned MIDIMelodyPlayer::getmilliseconds()
{
	unsigned m1,m2;
	xtime _t;
	xtime_get(&_t,TIME_UTC);
	m1 = _t.sec*MILLISECONDS_PER_SECOND;
	m2 = _t.nsec/NANOSECONDS_PER_MILLISECOND;
	return m1+m2;
}

} // namespace V2M

// END

