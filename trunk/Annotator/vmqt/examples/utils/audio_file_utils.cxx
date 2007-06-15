/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MonoAudioFileReader.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "audio_file_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    int load_audio(const char* fileName,Audio& out)
    {
		MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(fileName);
		MonoAudioFileReader infile;
		if (!infile.Configure(cfg)) return 1;

		out.SetSize(infile.GetHeader().GetSamples());

		infile.Start();
		infile.Do(out);
		infile.Stop();

		return 0;
    }

    int load_audio_st(const char* fileName,std::vector<Audio>& outputs)
    {
		MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile(fileName);
		MultiChannelAudioFileReader reader;
		if (!reader.Configure(cfg)) return 1;

		const AudioFileHeader & header = reader.GetHeader();
		if (header.GetChannels() != 2) return 1;

		TSize readSize = TSize(TData(header.GetLength()/1000.0)*header.GetSampleRate());
		if(reader.GetKind() == EAudioFileKind::ePCM)
		{
			readSize*=2;
		}

		outputs.resize(header.GetChannels());

		for(unsigned i = 0; i < outputs.size(); i++)
		{
			outputs[i].SetSize(readSize);
		}


		reader.Start();
		reader.Do(outputs); 
		reader.Stop();
	
		return 0;
    }
}

// END

