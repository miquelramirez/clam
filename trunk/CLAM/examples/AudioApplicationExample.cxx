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
#include "Multiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioApplication.hxx"

#include "TopLevelProcessing.hxx"

using namespace CLAM;

class MyIOAudioApplication:public AudioApplication
{
	void AppCleanup() {}
public:
	void AudioMain(void);	
};

void MyIOAudioApplication::AudioMain(void)
{
	try
	{
		unsigned int buffersize = 256;

		AudioManager audioManager(48000,4096);

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

		Audio bufTesttone;
		bufTesttone.SetSize(buffersize);

		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);

		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);

		OscillatorConfig oscRcfg;
		OscillatorConfig oscLcfg;
		
		oscRcfg.SetFrequency(0.2);
		oscLcfg.SetFrequency(0.2);
		oscLcfg.SetPhase(PI/2.);

		Multiplier mul;
		AudioAdder add;

		Oscillator oscL(oscRcfg);
		Oscillator oscR(oscLcfg);

		OscillatorConfig testtoneCfg;

		testtoneCfg.SetFrequency(440);

		Oscillator testtone(testtoneCfg);

		audioManager.Start();

		TopLevelProcessing::GetInstance()->Start();

		do
		{
			inL.Do(bufL);
			inR.Do(bufR);

			testtone.Do(bufTesttone);

			oscL.Do(bufOsc);
			mul.Do(bufL,bufOsc,bufL);

			oscR.Do(bufOsc);
			mul.Do(bufTesttone,bufOsc,bufTesttone);

			add.Do(bufTesttone,bufL,bufL);

			outL.Do(bufL);
			outR.Do(bufR);

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

class MyOutAudioApplication:public AudioApplication
{
	void AppCleanup() {}
public:
	void AudioMain(void);	
};

void MyOutAudioApplication::AudioMain(void)
{
	try
	{
		unsigned int buffersize = 256;

		AudioManager audioManager(48000,4096);

		AudioIOConfig outLCfg;
		outLCfg.SetName("left out");
		outLCfg.SetChannelID(0);

		AudioIOConfig outRCfg;
		outRCfg.SetName("right out");
		outRCfg.SetChannelID(1);

		Audio bufOsc;
		bufOsc.SetSize(buffersize);

		AudioOut outL(outLCfg);
		AudioOut outR(outRCfg);

		OscillatorConfig testtoneCfg;

		testtoneCfg.SetFrequency(440);

		Oscillator testtone(testtoneCfg);

		audioManager.Start();
		TopLevelProcessing::GetInstance()->Start();

		do
		{
			testtone.Do(bufOsc);
			outL.Do(bufOsc);
			outR.Do(bufOsc);
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

		{
			MyIOAudioApplication app;
			app.Run(argc,argv);
		}
		getchar();
		{
			MyOutAudioApplication app;
			app.Run(argc,argv);
		}


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
