#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include <vector>

using namespace CLAM;

void ConfigureAndCheck(Processing& p,ProcessingConfig& cfg)
{
	CLAM_ASSERT(p.Configure(cfg),p.GetStatus().c_str());
}

main()
{
	char* indevice = "file:test.mid";
	char* outdevice = "textfile:test.txt";
	
	MIDIManager manager;
	MIDIIOConfig inNoteCfg;
	MIDIIOConfig outNoteCfg;

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

	MIDIInControl inNote;
	ConfigureAndCheck(inNote,inNoteCfg);
	MIDIOutControl outNote;
	ConfigureAndCheck(outNote,outNoteCfg);
	
	
	//control for stoping at eof 
	
	MIDIIOConfig inStopCfg;
	inStopCfg.SetDevice(indevice);
	inStopCfg.SetChannel(CLAM::MIDI::eStop); //it is a sys message that uses channel byte for actual data
	inStopCfg.SetMessage(CLAM::MIDI::eSystem);
	
	MIDIInControl inStop;
	ConfigureAndCheck(inStop,inStopCfg);
	InControl stopReceiver("stop-receiver");

	inStop.GetOutControls().GetByNumber(0).AddLink(
			&stopReceiver);
	
	inNote.GetOutControls().GetByNumber(0).AddLink(
			&outNote.GetInControls().GetByNumber(0));
	inNote.GetOutControls().GetByNumber(1).AddLink(
			&outNote.GetInControls().GetByNumber(1));
	inNote.GetOutControls().GetByNumber(2).AddLink(
			&outNote.GetInControls().GetByNumber(2));
	
	manager.Start();

	TTime curTime = 0;

	while (stopReceiver.GetLastValue()==0)
	{
		//we send a timing control to the MIDI clocker 
		inpClocker.GetInControls().GetByNumber(0).DoControl(curTime);
		outClocker.GetInControls().GetByNumber(0).DoControl(curTime);
		
		//we check for new events in the MIDI manager
		manager.Check();
		
		//we increment the time counter
		curTime ++;
	}
}
