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
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "MIDILinker.hxx"
#include "Oscillator.hxx"
#include <iostream>
#include "AudioIO.hxx"

using namespace CLAM;

int main (int argc, char** argv)
{
	try
	{
		CLAM::XMLStorage storage;
		MIDILinkerConfig conf;

//restore config saved as xml
//		storage.Restore(conf,"midi.xml");

		AudioManager audioManager(48000,2048);
		Cmd_MIDILinker midi;
	
		unsigned int buffersize = 2048;
		OscillatorConfig oscCfg;

		oscCfg.SetFrequency(440.0);
		
		oscCfg.SetName("sinus1");
		Oscillator osc(oscCfg);
				
		oscCfg.SetName("sinus2");
		Oscillator osc2(oscCfg);
				
		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;
		
		inCfgL.SetName("left in");
		inCfgL.SetChannelID(0);
				
		inCfgR.SetName("right in");
		inCfgR.SetChannelID(1);
		
		outCfgL.SetName("left out");
		outCfgL.SetChannelID(0);
				
		outCfgR.SetName("right out");
		outCfgR.SetChannelID(1);
				
		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);
		
		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);
		
		Audio bufL;
		bufL.SetSize(buffersize);
		
		Audio bufR;
		bufR.SetSize(buffersize);
		
		Audio bufOsc;
		bufOsc.SetSize(buffersize);
		
		Audio bufOsc2;
		bufOsc2.SetSize(buffersize);
						
		cout << audioManager.Latency() << endl;				
		char* name1 = "sinus1";
		char* name2 = "sinus2";

////to restore saved configuration
//		Cmd_MIDILinker midi(conf);

		midi.cbShowProcessingControls(name1);
		midi.cbShowProcessingControls(name2);
//////to map note -> freq
		DataArray parametres(2);
		parametres.SetSize(2);
		parametres[0] = 69;
		parametres[1] = 440;
		parametres.Resize(2);

///////////////////////

///// to map linearly 0/127 -> 0/
//		DataArray parametres(4);
//		parametres.SetSize(4);

//		parametres[0] = 0.0;
//		parametres[1] = 127.0;
//		parametres[2] = 0.0;
//		parametres[3] = 1.0;

//		parametres.Resize(4);

	
		ControlMapperConfig mapperCfg = midi.CreateMapperConfig("mapper1","NoteToFreq",parametres);
		ControlMapperConfig mapperCfg2 = midi.CreateMapperConfig("mapper2","NoteToFreq",parametres);
//frequencies amb el noteon de cada un
		MIDIInConfig MIDICfg = midi.CreateMIDICfg(1,MIDI::eNoteOn,"midi1");
		MIDIInConfig MIDICfg2 = midi.CreateMIDICfg(1,MIDI::eNoteOff,"midi2");
//amplitud sera el velocity
		midi.LinkMIDIWithControl(MIDICfg,name1,0,mapperCfg);
		midi.LinkMIDIWithControl(MIDICfg2,name2,0,mapperCfg2);

//store config
		storage.Dump(midi.mConfig, "MIDIConfig", "midi.xml");
		
		audioManager.Start();
		MIDIManager::Singleton().Start();
		osc.Start();
		osc2.Start();
		
		while (1)
		{		
			inL.Do(bufL);
			inR.Do(bufR);	
			midi.Do();
			osc.Do(bufOsc);
			osc2.Do(bufOsc2);
			outL.Do(bufOsc);
			outR.Do(bufOsc2);
		}
		
	}
	catch( Err e )
	{
		cout << "Exception thrown: " << endl;
		e.Print();
	}
	return 0;
}
