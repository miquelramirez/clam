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

#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "GUIAudioApplication.hxx"

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

using namespace CLAM;

class MyAudioApplication:public GUIAudioApplication
{
	void AppCleanup() {}
public:
	void AudioMain(void);	
};

void MyAudioApplication::AudioMain(void)
{
	try
	{
		unsigned int buffersize = 512;
		AudioManager audioManager(44100, 512 );


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

		Audio bufL;
		bufL.SetSize(buffersize);
		Audio bufR;
		bufR.SetSize(buffersize);

		Audio bufOsc;
		bufOsc.SetSize(buffersize);

		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);

		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);

		OscillatorConfig oscRcfg;
		OscillatorConfig oscLcfg;
		
		OscillatorConfig testtoneCfg;

		oscRcfg.SetFrequency(0.2);
		oscLcfg.SetFrequency(0.2);
		oscLcfg.SetPhase(PI/2.);

		testtoneCfg.SetFrequency(440);

		AudioMultiplier mul;

		Oscillator oscL(oscRcfg);
		Oscillator oscR(oscLcfg);
		Oscillator testtone(testtoneCfg);

		//Starting all processings
		audioManager.Start();
		inL.Start();
		inR.Start();
		oscL.Start();
		oscR.Start();
		mul.Start();
		testtone.Start();
		outL.Start();
		outR.Start();

		do
		{
			printf("."); fflush(stdout);
			inL.Do(bufL);
			inR.Do(bufR);

			oscL.Do(bufOsc);
			mul.Do(bufL,bufOsc,bufL);

			oscR.Do(bufOsc);
			mul.Do(bufR,bufOsc,bufR);

#if 1
			/* just play a test tone at 440 hz instead of doing the realtime I/O with LFO */
			testtone.Do(bufOsc);
			outL.Do(bufOsc);
			outR.Do(bufOsc);
#else
			outL.Do(bufL);
			outR.Do(bufR);
#endif
		} while (!Canceled());
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		exit(-1);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		exit(-1);
	}
}

int main(int argc,char** argv)
{
	try
	{
		MyAudioApplication app;
		app.Run(argc,argv);
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		return -1;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}
