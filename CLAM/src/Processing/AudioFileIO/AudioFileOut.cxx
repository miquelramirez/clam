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

#include "AudioFileOut.hxx"
#include "ErrProcessingObj.hxx"
#include "SoundFileIO.hxx"
#include "AIFFFileIO.hxx"
#include "WaveFileIO.hxx"
#include "ErrSoundFileIO.hxx"

namespace CLAM {

	AudioFileOut::AudioFileOut() :
		mpSoundFileIO(0),
		mInput("Input",this)
	{
		Configure(AudioFileConfig());
	};

	AudioFileOut::AudioFileOut(const AudioFileConfig &c) :
		mpSoundFileIO(0),
		mInput("Input",this)
	{ 
		Configure(c);
	};

	bool AudioFileOut::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		
		if (mConfig.HasFilename()) {
			if (mConfig.GetFilename()=="") {
				AddConfigErrorMessage("Empty filename specified in config");
				return false;
			}
		}else{
			AddConfigErrorMessage("No filename specified in config");
			return false;
		}

		if (mpSoundFileIO) {
			mpSoundFileIO->Close();
			delete mpSoundFileIO;
			mpSoundFileIO = 0;
		}

		switch (mConfig.GetFiletype())
		{
			case EAudioFileType::eRaw:
				mpSoundFileIO = new SoundFileIO;
				break;			
			case EAudioFileType::eWave:
				mpSoundFileIO = new WaveFileIO;
				break;			
			case EAudioFileType::eAIFF:
				mpSoundFileIO = new AIFFFileIO;;
				break;			
			default:
				AddConfigErrorMessage("Unknown file type specified in config");
				return false;
		}

		if (!ConcreteStart()) // Just to see if the file is there.
		{
			delete(mpSoundFileIO);
			mpSoundFileIO=0;
			return false;
		}

		ConcreteStop();

		mInput.SetSize(mConfig.GetFrameSize());
		mInput.SetHop(mConfig.GetFrameSize());
		return true;
	}

	AudioFileOut::~AudioFileOut()
	{
		if(mpSoundFileIO) {
			mpSoundFileIO->Close();
			delete mpSoundFileIO;
		}
	}

	bool AudioFileOut::ConcreteStart()
	{
		SoundHeader header
			(int(mConfig.GetSampleRate()), mConfig.GetChannels());
		
		if (header.mChannels == 0)
		{
			header.mChannels = 1;
		}
	
		if (!mpSoundFileIO)
			return false;
		try {	
			mpSoundFileIO->Create(
				mConfig.GetFilename().c_str(),SoundFileIO::eWrite,header);
		}
		catch (ErrSoundFileIO& err)
		{
			AddConfigErrorMessage("Error opening file: ");
			AddConfigErrorMessage("err.what()");
			return false;
		}
		return true;
	}

	bool AudioFileOut::ConcreteStop()
	{
		if(mpSoundFileIO) {
			mpSoundFileIO->Close();
			return true;
		}
		return false;
	}

	bool AudioFileOut::Do(const Audio& in)
	{
		if( !AbleToExecute() ) return true;

		short tmp[256];

		CLAM_ASSERT(mpSoundFileIO->Header().mChannels==1,
			"AudioFileOut: Do(): Not a mono file");
		int n = in.GetBuffer().Size();
		int pos=0;
		const DataArray &array = in.GetBuffer();
		try {
			while (n)
			{			
				int m = n;
				if (m>256) m=256;
				short* sptr = tmp;
				int j=m;
				while (j--) {
					int v = int(array[pos++]*32768.);
					if (v&0x80000000) {
						if ((v&0xFFFF8000)!=0xFFFF8000) v=-32768;
					}
					else if (v&0xFFFF8000) v=32767;
					*sptr++ = v;
				}
				mpSoundFileIO->Write(tmp,m);
				n -= m;
			}
		}
		catch (ErrSoundFileIO& orig) {
			throw ErrProcessingObj(orig.what(),this);
		}
				
		return true;		
	}

	bool AudioFileOut::Do(const Audio& inL,const Audio& inR)
	{
		short tmp[256];

		CLAM_ASSERT(IsRunning(),
			"AudioFileOut: Do(): Not in execution mode");

		CLAM_ASSERT(mpSoundFileIO->Header().mChannels==2,
			"AudioFileOut: Do(): Not a stereo file");

		int n = inL.GetBuffer().Size();
		CLAM_ASSERT(n == inR.GetBuffer().Size(),
			"AudioFileOut: Do(): Audio L/R needs same size");

		int pos=0;
		DataArray &arrayL = inL.GetBuffer();
		DataArray &arrayR = inR.GetBuffer();
		try {
			while (n)
			{			
				int m = n*2; /* n is in frames, m is in samples */
				if (m>256) m=256;
				short* sptr = tmp;
				int j=m/2; /* j is in frames */
				while (j--) {
					int v;
				
					v = int(arrayL[pos]*32768.);
					if (v&0x80000000) {
						if ((v&0xFFFF8000)!=0xFFFF8000) v=-32768;
					}
					else if (v&0xFFFF8000) v=32767;
					*sptr++ = v;

					v = int(arrayR[pos]*32768.);
					if (v&0x80000000) {
						if ((v&0xFFFF8000)!=0xFFFF8000) v=-32768;
					}
					else if (v&0xFFFF8000) v=32767;
					*sptr++ = v;
				
					pos++;
				}
				mpSoundFileIO->Write(tmp,m);
				n -= m/2;
			}
		}
		catch (ErrSoundFileIO& orig) {
			throw ErrProcessingObj(orig.what(),this);
		}
				
		return true;		
	}

	bool AudioFileOut::Do(void)
	{
		bool res = Do(mInput.GetAudio());
		mInput.Consume();
		return res;		
	}
};//namespace CLAM

