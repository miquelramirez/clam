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
#include "AudioApplication.hxx"
#include "ThreadedBuffer.hxx"
#include "AudioFileIn.hxx"

#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

using namespace CLAM;

class AudioFilePlayerApplication:public AudioApplication
{
public:
	void AudioMain(void);	
	void AppCleanup(void) {}
};

void AudioFilePlayerApplication::AudioMain(void)
{
	printf("AUDIO THREAD: %d\n",getpid());

	AudioManager audioManager;
	
	AudioIOConfig outCfgL;
	AudioIOConfig outCfgR;

	outCfgL.SetName("left out");
	outCfgL.SetChannelID(0);
	
	outCfgR.SetName("right out");
	outCfgR.SetChannelID(1);

	Audio bufL;
	bufL.SetSize(512);

	Audio bufR;
	bufR.SetSize(512);

	AudioOut outL(outCfgL);
	AudioOut outR(outCfgR);

/*
	OscillatorConfig oscCfg;
	Oscillator osc(oscCfg);
*/
	AudioFileConfig infilecfg;

	infilecfg.SetName("filein");
	infilecfg.SetFilename("/mnt/hda8/foo.wav");
	infilecfg.SetFiletype(EAudioFileType::eWave);
	infilecfg.SetKeepFrameSizes(true);


	AudioFileIn filein(infilecfg);

	if (filein.GetExecState() != Processing::Ready)
	{
		std::cerr << filein.GetStatus() << std::endl;
		exit(1);
	}

	ThreadedBufferIn tb(&filein);

	tb.Start();

	audioManager.Start();

	int k = 0;
	
	do
	{
		if (k++>5000) exit(-1);
		tb.Do(bufL,bufR);
//		osc.Do(buf);
		outL.Do(bufL);
		outR.Do(bufR);
	} while (!Canceled());
}

#include <FL/Fl_Window.H>
#include <FL/Fl.H>

#ifndef WIN32
//#include "clam_su_wrapper.h"
#endif

int main(int argc,char** argv)
{
	try
	{
		AudioFilePlayerApplication app;
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
