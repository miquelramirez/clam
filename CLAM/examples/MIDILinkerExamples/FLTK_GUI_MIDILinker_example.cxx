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

#include "FLTK_GUI_MIDILinker.hxx"
#include "AudioIO.hxx"
#include "Oscillator.hxx"
#include <FL/Fl.H>

using namespace CLAM;

int main(){
	try{
		AudioManager audioManager(48000,512);
		FLTK_GUI_MIDILinker fgm ;
		
		unsigned int buffersize = 512;
		
		
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
		
		inCfgL.SetName("Left in channel");
		inCfgL.SetChannelID(0);
		
		inCfgR.SetName("Right in channel");
		inCfgR.SetChannelID(1);
		
		outCfgL.SetName("Left out channel");
		outCfgL.SetChannelID(0);
				
		outCfgR.SetName("Right out channel");
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

		fgm.Display();

		if(fgm.IsConfigured())
		{
			audioManager.Start();
			MIDIManager::Singleton().Start();		   
			osc.Start();
			osc2.Start();
				
			while (1)
			{		
				inL.Do(bufL);
				inR.Do(bufR);	
				fgm.Do();
				osc.Do(bufOsc);
				osc2.Do(bufOsc2);
				outL.Do(bufOsc);
				outR.Do(bufOsc2);
			}			
		}
	}
	catch( Err e )
	{
		cout << "Exception thrown: " << endl;
		e.Print();
	}
	return 0;
}
