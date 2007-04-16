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

#include<iostream>
#include<fstream>
#include<string>

#include "AudioMultiplier.hxx"
#include "CircularShift.hxx"
#include "WindowGenerator.hxx"
#include "TestUtils.hxx"

using namespace CLAM;
using namespace CLAMTest;
using namespace TestUtils;

void DumpAudio(Audio& a)
{
	int i;
	int size = a.GetSize();

	for (i=0;i<size;i++)
		std::cout << a.GetBuffer()[i]<< std::endl;

}



int main(int argc,char** argv)
{
	try {

		WindowGeneratorConfig wconf;
		wconf.SetSize(1024);
		wconf.SetNormalize(EWindowNormalize::eNone);
		WindowGenerator wingen(wconf);
		wconf.SetInvert(true);
		WindowGenerator invwingen(wconf);

		/* Audio Data Objects */

		int i,Size = 1024;
		Audio audioData1,audioData2,audioData3;
		audioData1.SetSize(Size);
		audioData2.SetSize(Size);
		audioData3.SetSize(Size);

		Audio audiowin;
		Audio invaudiowin;
		audiowin.SetSize(1024);
		invaudiowin.SetSize(1024);

		/* Fill them with live */

		for (i=0;i<Size;i++) {
#if 0
			audioData1.GetBuffer()[i]=1.0;
#endif
#if 0
			audioData1.GetBuffer()[i]=
				0.3*sin(2.0*PI*200.0*(((float)i)/SampleRate)) + 
				0.2*sin(2.0*PI*400.0*(((float)i)/SampleRate)) +  
				0.1*sin(2.0*PI*600.0*(((float)i)/SampleRate)) +  
				0.1*sin(2.0*PI*800.0*(((float)i)/SampleRate)) +  
				0.1*sin((2.0*PI*1000.0*((float)i)/SampleRate));
			if (i < 1024) audioData1.GetBuffer()[i]*= ((float)i/(float)1024);
			else audioData1.GetBuffer()[i]*= ((float)(Size - i)/(float)Size);
#endif
		}

		BinaryAudioOpConfig apconfig;
		AudioMultiplier aprod(apconfig);

		CircularShiftConfig sconfig;
		sconfig.SetAmount(-256);
		CircularShift       shift(sconfig);

		wingen.Start();
		aprod.Start();
		shift.Start();

		/* Fill the window objects with windows */
		wingen.SetSize(513);
		invwingen.SetSize(513);

		wingen.Do(audiowin);
		invwingen.Do(invaudiowin);

		aprod.Do(audioData1,audiowin,audioData2);

		shift.Do(audioData2,audioData2);
		shift.SetAmount(256);
		shift.Do(audioData2,audioData2);

		aprod.Do(audioData2,invaudiowin,audioData3);

		if (audioData3 != audioData1) {
			std::cout << "Audio objects differ after invariant transformation!" << std::endl;
			std::cout << std::endl << "Failed." << std::endl;
			return 1;
		}

		DumpAudio(audiowin);
		std::cout << std::endl << "Passed." << std::endl;

		return 0;
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << std::endl << "Failed." << std::endl;
		return 1;
	}
}

