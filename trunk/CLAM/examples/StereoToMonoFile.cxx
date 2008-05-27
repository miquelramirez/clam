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

#include "MultiChannelAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
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
		MultiChannelAudioFileReaderConfig infilecfg;
		infilecfg.SetSourceFile(argv[1]);
		MultiChannelAudioFileReader infile(infilecfg);

		double sampleRate = infile.GetHeader().GetSampleRate();

		MonoAudioFileWriterConfig outfilecfgL;
		outfilecfgL.SetTargetFile(argv[2]);
		outfilecfgL.SetSampleRate(sampleRate);
		MonoAudioFileWriter outfileL(outfilecfgL);

		MonoAudioFileWriterConfig outfilecfgR;
		outfilecfgR.SetTargetFile(argv[3]);
		outfilecfgR.SetSampleRate(sampleRate);
		MonoAudioFileWriter outfileR(outfilecfgR);

		std::vector<Audio> buffers(2);
		buffers[0].SetSize(512);
		buffers[1].SetSize(512);

		infile.Start();
		outfileL.Start();
		outfileR.Start();

		while (infile.Do(buffers))
		{
			outfileL.Do(buffers[0]);
			outfileR.Do(buffers[1]);
		}

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
	
