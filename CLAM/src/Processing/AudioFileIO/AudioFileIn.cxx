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
#include "ErrProcessingObj.hxx"
#include "SoundFileIO.hxx"
#include "AIFFFileIO.hxx"
#include "WaveFileIO.hxx"
#include "ErrSoundFileIO.hxx"

using namespace CLAM;

	AudioFileIn::AudioFileIn() :
		mpSoundFileIO(0),
		mOutput("Output",this,1)
	{
		Configure(AudioFileConfig());
	};

	AudioFileIn::AudioFileIn(const AudioFileConfig &c) :
		mpSoundFileIO(0),
		mOutput("Output",this,1)
	{ 
		Configure(c);
	};

	bool AudioFileIn::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		
		if (!mConfig.HasFilename()) {
			mStatus += "No filename specified in config\n";
			return false;
		}
		if (mConfig.GetFilename()=="") {
			mStatus += "Empty filename specified in config\n";
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
				mStatus += "Unknown file type specified in config\n";
				return false;
		}

		if (!ConcreteStart()) // Just to see if the file is there.
		{
			delete(mpSoundFileIO);
			mpSoundFileIO=0;
			return false;
		}

		ConcreteStop();

		mKeepFrameSizes = mConfig.GetKeepFrameSizes();
		
		mOutput.SetParams(mConfig.GetFrameSize());

		return true;
	}

	AudioFileIn::~AudioFileIn()
	{
		if (mpSoundFileIO)
		{
			delete mpSoundFileIO;
			mpSoundFileIO = 0;
		}
	}

	bool AudioFileIn::Do(Audio& in)
	{
		if( !AbleToExecute() ) return true;
		
		short tmp[256];

		CLAM_ASSERT(mpSoundFileIO->Header().mChannels==1,
			"AudioFileIn: Do(): Not a mono file");

		int n = mpSoundFileIO->Size()-mpSoundFileIO->TellFrame();
		int m = in.GetBuffer().Size();

		if (n>m)
			n=m;
		else
			m=n;

		TData* ptr = in.GetBuffer().GetPtr();
		while (n)
		{			
			int j = n;
			if (j>256) j=256;
			try {
				mpSoundFileIO->Read(tmp,j);
			}
			catch (ErrSoundFileIO e) {
				throw ErrProcessingObj(e.mStr,this);
			}
			short* sptr = tmp;
			n -= j;
			while (j--) {
				*ptr++ = (*sptr++)/TData(32768.);
			}
		}
		
		if (!mKeepFrameSizes)
		{
			in.SetSize(m);
		}
		else
		{
			AddSilence(in,m);
		}
				
		if (m)
			return true;
		return false;
	}

	void AudioFileIn::AddSilence(Audio& audio, int valid_data)
	{
		int i,size = audio.GetSize();
		Array<TData> &a = audio.GetBuffer();
		for (i=valid_data; i<size; i++)
			a[i]=0.0;
	}

	bool AudioFileIn::Do(Audio& inL,Audio& inR)
	{
		short tmp[256];

		if ( GetExecState() == Unconfigured ||
			 GetExecState() == Ready )
			throw(ErrProcessingObj("AudioFileOut: Do(): Not in execution mode",this));

		if (mpSoundFileIO->Header().mChannels!=2)
			throw(ErrProcessingObj("AudioFileIn: Do(): Not a stereo file",this));

		int n = mpSoundFileIO->SizeFrame()-mpSoundFileIO->TellFrame();
		int m = inL.GetBuffer().Size();
		if (m != inR.GetBuffer().Size())
		{
			throw(ErrProcessingObj("AudioFileOut: Do(): Audio L/R needs same size",this));
		}
		
		if (n>m)
			n=m;
		else
			m=n;

		TData* ptrL = inL.GetBuffer().GetPtr();
		TData* ptrR = inR.GetBuffer().GetPtr();
		while (n)
		{			
			int j = n*2; /* n in number of frames, j in number of samples */
			if (j>256) j=256;
			mpSoundFileIO->Read(tmp,j);
			short* sptr = tmp;
			j /= 2; /* j now also in number of frames */
			n -= j;
			while (j--) {
				*ptrL++ = (*sptr++)/TData(32768.);
				*ptrR++ = (*sptr++)/TData(32768.);
			}
		}

		if (!mKeepFrameSizes)
		{
			inL.SetSize(m);
			inR.SetSize(m);
		}
		else
		{
			AddSilence(inL,m);
			AddSilence(inR,m);
		}
				
		if (m)
			return true;
		return false;
	}

	bool AudioFileIn::Do(void)
	{
		bool res = Do(mOutput.GetData());
		mOutput.LeaveData();
		return res;
	}

	bool AudioFileIn::ConcreteStart()
	{
		if (!mpSoundFileIO)
			return false;
		try {	
			mpSoundFileIO->Open(mConfig.GetFilename().c_str(),WaveFileIO::eRead);
		}
		catch (ErrSoundFileIO err)
		{
			mStatus += "Error opening file: ";
			mStatus += err.mStr;
			mStatus += "\n";
			return false;
		}
		if (
			mConfig.GetChannels() && 
			mConfig.GetChannels()!=mpSoundFileIO->Header().mChannels
		)
		{
			mStatus += "AudioFileIn: File does not have the requested number of channels";
			return false;
		}
		try {	
			mpSoundFileIO->SeekFrame(mConfig.GetStartFrame());
//			printf("Seeking to %d\n",mConfig.GetStartFrame());
		}
		catch (ErrSoundFileIO&)
		{
			mStatus = "Error seeking frame\n";
			return false;
		}	
		return true;
	}

	bool AudioFileIn::ConcreteStop()
	{
		if(mpSoundFileIO)
		{
			mpSoundFileIO->Close();
			return true;
		}
		return false;
	}



	bool AudioFileIn::Done(void)
	{
		return mpSoundFileIO->Tell()>=mpSoundFileIO->Size();
	}
	int AudioFileIn::Size()
	{
		return mpSoundFileIO->Size();
	}
	TData AudioFileIn::SampleRate()
	{
		return (TData) mpSoundFileIO->SampleRate();
	}
	int AudioFileIn::Channels()
	{
		return mpSoundFileIO->Header().mChannels;
	}

