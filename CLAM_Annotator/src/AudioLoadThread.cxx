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


#include "AudioLoadThread.hxx"
#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>

unsigned AudioLoadThread::number()
{
	static unsigned number = 0;
	return number++;
}

AudioLoadThread::AudioLoadThread(CLAM::Audio & audio, const std::string audioFileName)
	: mAborted(false)
	, mAudio(audio)
	, readSize(4096)
	, mNumber(number())
{
	std::cout << "AudioLoader " << mNumber << " created..." << std::endl;
	file.OpenExisting(audioFileName);
	mAudio.SetSize(0);
	if (!file.IsReadable()) return; // Exits with audio size = 0;

	CLAM::TData samplingRate = file.GetHeader().GetSampleRate();
	nSamples = unsigned( file.GetHeader().GetLength()/1000.0*samplingRate );
	int nChannels = file.GetHeader().GetChannels();
	audioFrameVector.resize(nChannels);
	for (int i=0;i<nChannels;i++)
		audioFrameVector[i].SetSize(readSize);
	mAudio.SetSize(nSamples);
	mAudio.SetSampleRate(samplingRate);
}
AudioLoadThread::~AudioLoadThread()
{
	std::cout << "Aborting audio loader " << mNumber << "..." << std::endl;
	mAborted=true;
	wait();
	std::cout << "AudioLoader " << mNumber << " destroyed..." << std::endl;
}
void AudioLoadThread::run()
{
	std::cout << "AudioLoader " << mNumber << " running..." << std::endl;
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	cfg.SetSourceFile( file );
	CLAM::MultiChannelAudioFileReader reader(cfg);
	reader.Start();
	int beginSample=0;
	while(reader.Do(audioFrameVector))
	{
		mAudio.SetAudioChunk(beginSample,audioFrameVector[0]);
		beginSample+=readSize;
		if (mAborted) break;
		if ( beginSample+readSize > nSamples ) break;
	}
//		mAudio.SetSize(beginSample);
	reader.Stop();
	std::cout << "AudioLoader " << mNumber << " finished..." << std::endl;
}

