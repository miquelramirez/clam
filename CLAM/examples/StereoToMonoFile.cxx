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

#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include <stdio.h>

using namespace CLAM;

int main(int argc,char** argv)
{
	if (argc!=4)
	{
		printf(
"Usage: StereoToMonoFile stereo-inputfile left-outfile right-outfile\n");
		return -1;
	}
	
	try {
		AudioFileConfig infilecfg;
		AudioFileConfig outfilecfgL,outfilecfgR;

		infilecfg.SetFilename(argv[1]);
		infilecfg.SetFiletype(EAudioFileType::eWave);

		outfilecfgL.SetFilename(argv[2]);
		outfilecfgL.SetFiletype(EAudioFileType::eWave);

		outfilecfgR.SetFilename(argv[3]);
		outfilecfgR.SetFiletype(EAudioFileType::eWave);

		AudioFileIn infile(infilecfg);
		AudioFileOut outfileL(outfilecfgL);
		AudioFileOut outfileR(outfilecfgR);

		Audio bufL;
		Audio bufR;
		bufL.SetSize(512);
		bufR.SetSize(512);

		infile.Start();
		outfileL.Start();
		outfileR.Start();

		do
		{
			infile.Do(bufL,bufR);
			outfileL.Do(bufL);
			outfileR.Do(bufR);
		} while (!infile.Done());

		infile.Stop();
		outfileL.Stop();
		outfileR.Stop();
	}	
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	return 0;
}
	
