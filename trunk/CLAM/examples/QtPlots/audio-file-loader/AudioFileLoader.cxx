/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioFileLoader.hxx"
#include "MonoAudioFileReader.hxx"
#include "MultiChannelAudioFileReader.hxx"

using namespace CLAM;

AudioFileLoader::AudioFileLoader(){}
AudioFileLoader::~AudioFileLoader(){}

int AudioFileLoader::Load(const char* fileName,Audio& out)
{
	AudioFile file;
	file.OpenExisting(fileName);

	if((!file.IsReadable()) | (file.GetHeader().GetChannels() > 1))
		return 1; 

	out.SetSize(file.GetHeader().GetSamples());

	MonoAudioFileReaderConfig cfg;
	cfg.SetSourceFile(file);

	MonoAudioFileReader infile;
	infile.Configure(cfg);

	infile.Start();
	infile.Do(out);
	infile.Stop();

	return 0;
}

int AudioFileLoader::LoadST(const char* fileName,std::vector<Audio>& outputs)
{
	AudioFile file;
	file.OpenExisting(fileName);

	if((!file.IsReadable()) | (file.GetHeader().GetChannels() != 2)) return 1;

	TSize readSize = TSize(TData(file.GetHeader().GetLength()/1000.0)*file.GetHeader().GetSampleRate());
	if(file.GetKind() == EAudioFileKind::ePCM)
	{
		readSize*=2;
	}

	outputs.resize(file.GetHeader().GetChannels());

	for(unsigned i = 0; i < outputs.size(); i++)
	{
		outputs[i].SetSize(readSize);
	}

	MultiChannelAudioFileReaderConfig cfg;
	cfg.SetSourceFile(file);

	MultiChannelAudioFileReader reader;
	reader.Configure(cfg);

	reader.Start();
	reader.Do(outputs); 
	reader.Stop();
	
	return 0;
}

// END

