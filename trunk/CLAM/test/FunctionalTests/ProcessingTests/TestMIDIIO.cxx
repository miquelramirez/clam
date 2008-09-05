#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include <vector>

using namespace CLAM;

main()
{
	char* indevice = "file:zefile.mid";
	char* outdevice = "textfile:test.txt";
	
	MIDIManager manager;
	MIDIInConfig inNoteCfg;
	MIDIOutConfig outNoteCfg;

	MIDIClockerConfig inpClockerCfg;
	MIDIClockerConfig outClockerCfg;

	inpClockerCfg.SetDevice(indevice);
	outClockerCfg.SetDevice(outdevice);
		
	MIDIClocker inpClocker(inpClockerCfg);
	MIDIClocker outClocker(outClockerCfg);

	inNoteCfg.SetDevice(indevice);
	inNoteCfg.SetMessage(MIDI::eNoteOnOff);
	
	outNoteCfg.SetDevice(outdevice);
	outNoteCfg.SetMessage(MIDI::eNoteOnOff);

	MIDIInControl inNote(inNoteCfg);
	MIDIOutControl outNote(outNoteCfg);
	
	//control for stoping at eof 
	
	MIDIInConfig inStopCfg;
	inStopCfg.SetDevice(indevice);
	inStopCfg.SetChannel(CLAM::MIDI::eStop); //it is a sys message that uses channel byte for actual data
	inStopCfg.SetMessage(CLAM::MIDI::eSystem);
	
	MIDIInControl inStop(inStopCfg);
	InControl stopReceiver("stop-receiver");

	inStop.GetOutControl(0).AddLink( &stopReceiver);
	inNote.GetOutControl(0).AddLink( &outNote.GetInControl(0));
	inNote.GetOutControl(1).AddLink( &outNote.GetInControl(1));
	inNote.GetOutControl(2).AddLink( &outNote.GetInControl(2));
	
	manager.Start();

	TTime curTime = 0;

	while (stopReceiver.GetLastValue()==0)
	{
		//we send a timing control to the MIDI clocker 
		SendFloatToInControl(inpClocker,0,curTime);
		SendFloatToInControl(outClocker,0,curTime);
		
		//we check for new events in the MIDI manager
		manager.Check();
		
		//we increment the time counter
		curTime ++;
	}
}

