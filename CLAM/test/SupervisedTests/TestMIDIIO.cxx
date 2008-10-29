#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"
#include "Processing.hxx"
#include <vector>

using namespace CLAM;

void ConfigureAndCheck(Processing& p,ProcessingConfig& cfg)
{
	CLAM_ASSERT( p.Configure(cfg), p.GetConfigErrorMessage().c_str() );
}

int main()
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
	FloatInControl stopReceiver("stop-receiver");

	inStop.GetOutControl(0).AddLink(stopReceiver);
	ConnectControls(inNote, 0, outNote, 0);
	ConnectControls(inNote, 1, outNote, 1);
	ConnectControls(inNote, 2, outNote, 2);
	
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
	return 0;
}

