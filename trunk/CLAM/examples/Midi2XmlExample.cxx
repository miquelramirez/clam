/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GeMidi2XmlExampleneral Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "MIDI2Melody.hxx"
#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIClocker.hxx"
#include "MIDIMelody.hxx"
#include "TraverseDirectory.hxx"

#include "MIDIReader.hxx"
#include <iostream>

//using namespace CLAM;
using MIDI::Reader;

using CLAM::MIDIMelody;
using CLAM::MIDIManager;
using CLAM::MIDIInConfig;
using CLAM::MIDIIn;
using CLAM::MIDIClockerConfig;
using CLAM::MIDIClocker;
using CLAM::MIDI2Melody;
using CLAM::TTime;
using CLAM::MIDIInControl;
using CLAM::XMLStorage;

/** Declaration of the concrete Traverse directory class to apply to all files*/
class ConvertAllMidiFiles:public TraverseDirectory
{
public:
		void OnFile(const std::string& filename);
private:
		std::string ChangeExtension(const std::string& filename,const std::string& newExtension);
};


//Auxiliary function to change the .mid extension to .xml in a given filename
std::string ConvertAllMidiFiles::ChangeExtension(const std::string& filename,const std::string& newExtension)
{
	std::string result=filename.substr(0,filename.rfind('.')+1);
	result.append(newExtension);
	return result;
}

//Function where the process related to every file is actually implemented
void ConvertAllMidiFiles::OnFile(const std::string& filename)
{      
	std::string mid("mid");
	
	//First we ensure that it is a MIDI file looking at the extension
	if(GetExtension(filename)!=mid) 
		return;
	std::cout<<"\n"<<"Processing file: "<<filename<<"\n";

	//We configure MIDIManager and controls
	std::string midiDeviceStr="file:"+filename;
	MIDIManager manager;
	// Control for handling MIDI notes		
	MIDIInConfig inNoteCfg;	
	inNoteCfg.SetDevice(midiDeviceStr);
	inNoteCfg.SetChannelMask(CLAM::MIDI::ChannelMask(-1)); //all
	inNoteCfg.SetMessageMask(CLAM::MIDI::MessageMask(CLAM::MIDI::eNoteOn)|CLAM::MIDI::MessageMask(CLAM::MIDI::eNoteOff));
		
	MIDIInControl inNote(inNoteCfg);
  
	//control for stoping at eof 
	MIDIInConfig inStopCfg;
	inStopCfg.SetDevice(midiDeviceStr);
	inStopCfg.SetChannelMask(CLAM::MIDI::SysMsgMask(CLAM::MIDI::eStop)); //it is a sys message that uses channel byte for actual data
	inStopCfg.SetMessageMask(CLAM::MIDI::MessageMask(CLAM::MIDI::eSystem));
	
	MIDIInControl inStop(inStopCfg);

	//MIDI clocker to keep track of timing
	MIDIClockerConfig clockerCfg;

	clockerCfg.SetDevice(midiDeviceStr);
		
	MIDIClocker clocker(clockerCfg);

	//We instantiate our MIDI2Melody converter and link MIDi controls to its input controls
	MIDI2Melody converter;

	/** Key for Note Off */
	inNote.GetOutControls().GetByNumber(0).AddLink(&converter.GetInControls().GetByNumber(2));
	/** Velocity for Note Off */
	inNote.GetOutControls().GetByNumber(1).AddLink(&converter.GetInControls().GetByNumber(3));
	/** Key for Note On */
	inNote.GetOutControls().GetByNumber(2).AddLink(&converter.GetInControls().GetByNumber(4));
	 /** Velocity for Note On */
	inNote.GetOutControls().GetByNumber(3).AddLink(&converter.GetInControls().GetByNumber(5));
  
	inStop.GetOutControls().GetByNumber(0).AddLink(&converter.GetInControls().GetByNumber(0));
  
	//We start the MIDI manager and initialize loop variables	
	manager.Start();
	TTime curTimeInc=10;
	TTime curTime=0;

	//We attach our melody object to the output of the converter
	MIDIMelody outputMelody;
	converter.mOutput.Attach(outputMelody);

	do{//Converter loop
		
			//we send a timing control to the MIDI clocker 
			clocker.GetInControls().GetByNumber(0).DoControl(curTime);
			//we increment the time counter
			curTime += curTimeInc;
			//we check for new events in the MIDI manager
			manager.Check();
			//we send the timing control to converter
			converter.mTime.DoControl(curTime);
	}while (converter.Do());//we call the converter

	//we change the extension of the filename to .xml
	std::string outputFilename=ChangeExtension(filename,"xml");
	//we store the output melody
	XMLStorage s;
	s.Dump(outputMelody,"MidiMelody",outputFilename);
	std::cout<<"Wrote succesfully "<<outputFilename<<"\n";

}


//Main program
int main(int argc,char** argv)
{
	
	ConvertAllMidiFiles fileConverter;

	std::string dir;
	if (argc>1)//if program is called with a list of directories to look in...
	{
		for (int i=1;i<argc;i++)
		{
				//we recursively look on every file on the folder applying the OnFileFunc to each file
				fileConverter.Traverse(argv[i]);
		}
	}else{
		std::cout<<"Please enter path to directory where midi files are located (all subdirectories will also be scaned recursively)"<<"\n";
		std::cin>>dir;
		//we recursively look on every file on the folder applying the OnFileFunc to each file
		fileConverter.Traverse(dir);
	}
	return 0;
}





