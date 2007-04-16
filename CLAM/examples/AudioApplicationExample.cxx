/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
		unsigned int buffersize = 4096;

		AudioManager audioManager(44100,4096);

		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;

		inCfgL.SetChannelID(0);

		inCfgR.SetChannelID(1);

		outCfgL.SetChannelID(0);

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

		AudioMultiplier mul;
		AudioAdder add;

		Oscillator oscL(oscRcfg);
		Oscillator oscR(oscLcfg);

		OscillatorConfig testtoneCfg;

		testtoneCfg.SetFrequency(440);

		Oscillator testtone(testtoneCfg);

		audioManager.Start();

		TopLevelProcessing::GetInstance().Start();

		do
		{
			inL.Do(bufL);
			inR.Do(bufR);

			testtone.Do(bufTesttone);

			oscL.Do(bufOsc);
			mul.Do(bufL,bufOsc,bufL);

			oscR.Do(bufOsc);
			mul.Do(bufR,bufOsc,bufR);

			outL.Do(bufL);
			outR.Do(bufR);

		} while (!Canceled());
		
		TopLevelProcessing::GetInstance().Stop();
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
		unsigned int buffersize = 1024;
		int samplerate = 44100;
		
		AudioManager audioManager(samplerate,2048);

		AudioIOConfig outLCfg;
		outLCfg.SetDevice("rtaudio:default");
		outLCfg.SetChannelID(0);
		outLCfg.SetSampleRate(samplerate);

		AudioIOConfig outRCfg;
		outRCfg.SetDevice("rtaudio:default");
		outRCfg.SetChannelID(1);
		outRCfg.SetSampleRate(samplerate);

		Audio bufOsc;
		bufOsc.SetSize(buffersize);

		AudioOut outL(outLCfg);
		AudioOut outR(outRCfg);

		OscillatorConfig testtoneCfg;

		testtoneCfg.SetFrequency(440);
		testtoneCfg.SetSamplingRate(audioManager.SampleRate());

		Oscillator testtone(testtoneCfg);

		audioManager.Start();

		TopLevelProcessing::GetInstance().Start();

		do
		{
			testtone.Do(bufOsc);
			outL.Do(bufOsc);
			outR.Do(bufOsc);
		} while (!Canceled());
		
		TopLevelProcessing::GetInstance().Stop();

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
			MyOutAudioApplication app;
			app.Run(argc,argv);
		}
		{
			MyIOAudioApplication app;
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

