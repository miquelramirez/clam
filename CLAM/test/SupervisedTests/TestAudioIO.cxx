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
#include "AudioManager.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"

#ifndef WIN32
#include <sched.h>
#include <unistd.h>
#endif
#include <iostream>

using namespace CLAM;

#ifndef WIN32
void setscheduler(void)
{
	struct sched_param sched_param;

	if (sched_getparam(0, &sched_param) < 0) {
		printf("Scheduler getparam failed...\n");
		return;
	}
	sched_param.sched_priority = sched_get_priority_max(SCHED_RR);
	if (!sched_setscheduler(0, SCHED_RR, &sched_param)) {
		printf("Scheduler set to Round Robin with priority %i...\n", sched_param.sched_priority);
		fflush(stdout);
		return;
	}
	printf("!!!Scheduler set to Round Robin with priority %i FAILED!!!\n", sched_param.sched_priority);
	if (getuid()!=0)
	{
		printf("execute as root!\n");
	}
}
#endif

void test_stereo_io(void)
{
	AudioManager audioManager(44100, 512);
	
	AudioIOConfig inCfgL;
	AudioIOConfig inCfgR;
	AudioIOConfig outCfgL;
	AudioIOConfig outCfgR;

	inCfgL.SetChannelID(0);
	
	inCfgR.SetChannelID(1);

	outCfgL.SetChannelID(0);
	
	outCfgR.SetChannelID(1);

	Audio bufL;
	Audio bufR;
	bufL.SetSize(512);
	bufR.SetSize(512);

	AudioIn inL(inCfgL);
	AudioIn inR(inCfgR);

	AudioOut outL(outCfgL);
	AudioOut outR(outCfgR);

	audioManager.Start();
	inL.Start();
	inR.Start();
	outL.Start();
	outR.Start();

	do
	{
		inL.Do(bufL);
		inR.Do(bufR);
		
		outL.Do(bufL);
		outR.Do(bufR);
	} while (1);
}

void test_mono_io(void)
{
	AudioManager audioManager(44100, 512);

	AudioIOConfig inCfg;
	AudioIOConfig outCfg;

	Audio buf;
	buf.SetSize(512);

	AudioIn in(inCfg);

	AudioOut out(outCfg);

	audioManager.Start();
	in.Start();
	out.Start();

	do
	{
		in.Do(buf);
		out.Do(buf);
	} while (1);
}


#ifndef WIN32
//#include "clam_su_wrapper.h"
#endif

int main(int argc,char** argv)
{
#ifndef WIN32
	setscheduler();
#endif

	try {
		test_stereo_io();
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

