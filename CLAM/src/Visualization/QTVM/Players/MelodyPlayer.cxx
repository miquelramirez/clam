#include "AudioIO.hxx" 
#include "AudioManager.hxx" 
#include "AudioOut.hxx" 
#include "SimpleOscillator.hxx" 
#include "MelodyPlayer.hxx" 

namespace CLAM
{
    namespace VM
    {
	MelodyPlayer::MelodyPlayer()
	    : mAudioPtr(0), 
	      mSampleRate(TData(44100.0)),
	      mDuration(TData(0.0)),
	      mPlayMelody(true),
	      mPlayAudio(false)
	{
	    mMelody.AddNumberOfNotes();
	    mMelody.UpdateData();
	    mMelody.SetNumberOfNotes(0);

	    HaveData(true);

	    _thread.SetThreadCode(makeMemberFunctor0((*this), MelodyPlayer, thread_code));
	}
		
	MelodyPlayer::~MelodyPlayer()
	{
	}
		
	void MelodyPlayer::SetData(const Melody& melody, const TData& dur)
	{
	    mMelody = melody;
	    mDuration = dur;
	}

	Melody& MelodyPlayer::GetMelody()
	{
	    return mMelody;
	}
		
	void MelodyPlayer::thread_code()
	{
	    if(!mMelody.GetNumberOfNotes()) return;

	    TSize frameSize = 512;                    
	    if(mAudioPtr) mSampleRate = mAudioPtr->GetSampleRate();
	    AudioManager manager((int)mSampleRate,(int)frameSize);  

	    AudioOut channelL;   
	    AudioIOConfig audioOutCfgL;     
	    audioOutCfgL.SetChannelID(0);    
	    channelL.Configure(audioOutCfgL); 

	    AudioOut channelR;   
	    AudioIOConfig audioOutCfgR;     
	    audioOutCfgR.SetChannelID(1);    
	    channelR.Configure(audioOutCfgR); 
			
			
	    AudioManager::Current().Start();                            
	    channelL.Start();  
	    channelR.Start();

	    SimpleOscillatorConfig oscCfg;
	    oscCfg.SetSamplingRate(mSampleRate);
	    oscCfg.SetAmplitude(TData(0.6));
	    SimpleOscillator osc(oscCfg);
			
	    InControl& freqControl = osc.GetInControls().Get("Pitch");
    
	    Audio samplesL;             
	    Audio samplesR;
	    samplesL.SetSize(frameSize);
	    samplesR.SetSize(frameSize);
				
	    TIndex firstIndex=GetNoteIndex();
	    TIndex lastIndex=GetNoteIndex(false);
	    TIndex k=firstIndex;

	    TIndex start = int(_time.GetBegin()*mSampleRate);
	    int nSamples = start+int((mMelody.GetNoteArray()[lastIndex].GetTime().GetEnd()-mMelody.GetNoteArray()[firstIndex].GetTime().GetBegin())*mSampleRate);
	    
	    TIndex leftIndex = start;        
	    TIndex rightIndex = leftIndex+frameSize;

	    osc.Start();

	    for(TIndex i=start; i < nSamples; i+=frameSize)
	    {
		if(IsPaused())
		{
		    _time.SetBegin(TData(i)/mSampleRate);
		    SetPlaying(false);
		}
		if(!IsPlaying()) break;

		if(TData(i/mSampleRate) >= mMelody.GetNoteArray()[k].GetTime().GetEnd()) k++;

		if(mPlayMelody)
		{
		    if(TData(i/mSampleRate) >= mMelody.GetNoteArray()[k].GetTime().GetBegin() &&
		       TData(i/mSampleRate) < mMelody.GetNoteArray()[k].GetTime().GetEnd())
		    {
			freqControl.DoControl(mMelody.GetNoteArray()[k].GetFundFreq());
		    }
		    else
		    {
			freqControl.DoControl(0);
		    }
		}
		else
		{
		    freqControl.DoControl(0);
		}

		if(mPlayMelody && mPlayAudio)
		{
		    osc.Do(samplesL);
		    if(mAudioPtr) mAudioPtr->GetAudioChunk(leftIndex,rightIndex,samplesR);
		}		
		else
		{ 
		    if(mPlayAudio)
		    {
			if(mAudioPtr) mAudioPtr->GetAudioChunk(leftIndex,rightIndex,samplesL);
		    }
		    else
		    {
			osc.Do(samplesL);
		    }
		}

		if(mPlayMelody && mPlayAudio)
		{
		    channelL.Do(samplesL);
		    channelR.Do(samplesR);
		}
		else
		{
		    channelL.Do(samplesL);
		    channelR.Do(samplesL);
		}

		leftIndex += frameSize;
		rightIndex += frameSize;
	    }
	    osc.Stop();
	    channelL.Stop(); 
	    channelR.Stop();
	    if(!IsPaused()) _time.SetBegin(GetBeginTime());
	}

	TIndex MelodyPlayer::GetNoteIndex(bool first)
	{
	    int nNotes = mMelody.GetNumberOfNotes();
	    if(nNotes==1)
	    {
		return 0;
	    }
	    TData searchValue;
	    if(first)
	    {
		searchValue = _time.GetBegin();
	    }
	    else
	    {
		searchValue = _time.GetEnd();
	    }

	    if(searchValue <= mMelody.GetNoteArray()[0].GetTime().GetBegin()) return 0;
	    if(searchValue >= mMelody.GetNoteArray()[nNotes-1].GetTime().GetEnd()) return nNotes-1;
	    if(searchValue >= mMelody.GetNoteArray()[0].GetTime().GetBegin() &&
	       searchValue <= mMelody.GetNoteArray()[0].GetTime().GetEnd()) return 0;
	    if(searchValue >= mMelody.GetNoteArray()[nNotes-1].GetTime().GetBegin() &&
	       searchValue <= mMelody.GetNoteArray()[nNotes-1].GetTime().GetEnd())
		return nNotes-1;

	    TIndex index = 0;
	    TIndex currentIndex = 0;
	    TIndex left_index = 0;
	    TIndex right_index = nNotes-1;
	    while(left_index <= right_index)
	    {
		currentIndex = (left_index+right_index)/2;
		if(searchValue >= mMelody.GetNoteArray()[currentIndex].GetTime().GetBegin() &&
		   searchValue <= mMelody.GetNoteArray()[currentIndex].GetTime().GetEnd())
		{
		    index=currentIndex;
		    break;
		}
		if(searchValue < mMelody.GetNoteArray()[currentIndex].GetTime().GetBegin())
		{
		    right_index = currentIndex-1;
		}
		else if(searchValue > mMelody.GetNoteArray()[currentIndex].GetTime().GetBegin())
		{
		    left_index = currentIndex+1;
		}
	    }
	    return index;
	}

	void MelodyPlayer::UpdateNotePitch(const TIndex& index, const TData& newPitch)
	{
	    int nNotes = mMelody.GetNumberOfNotes();
	    if(!nNotes || index < 0 || index > nNotes-1) return;
	    mMelody.GetNoteArray()[index].SetFundFreq(newPitch);
	}

	void MelodyPlayer::UpdateNoteDuration(const TIndex& index, const TData& beginTime)
	{
	    int nNotes = mMelody.GetNumberOfNotes();
	    if(!nNotes || index < 0 || index > nNotes-1) return;
	    mMelody.GetNoteArray()[index].GetTime().SetBegin(beginTime);
	    if(index > 0) mMelody.GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
	}

	void MelodyPlayer::AddNote(const TIndex& index, const TData& beginTime, const TData& pitch)
	{
	    bool addElem = false;
	    Note note;
	    MediaTime time;
	    time.SetBegin(beginTime);

	    if(mMelody.GetNumberOfNotes())
	    {
		if(index >= mMelody.GetNumberOfNotes())
		{
		    time.SetEnd(mMelody.GetNoteArray()[mMelody.GetNumberOfNotes()-1].GetTime().GetEnd());
		    addElem = true;
		}
		else
		{
		    time.SetEnd(mMelody.GetNoteArray()[index].GetTime().GetBegin());
		}
	    }
	    else
	    {
		time.SetEnd(mDuration);
		_time.SetEnd(time.GetEnd());
	    }

	    if(index > 0 && mMelody.GetNumberOfNotes())
	    {
		 if(addElem)
		 {
		     mMelody.GetNoteArray()[mMelody.GetNumberOfNotes()-1].GetTime().SetEnd(beginTime);
		 }
		 else
		 {
		     mMelody.GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
		 }
	    }
	    
	    note.SetFundFreq(pitch);
	    note.SetTime(time);

	    if(addElem || !mMelody.GetNumberOfNotes())
	    {
		mMelody.GetNoteArray().AddElem(note);
	    }
	    else
	    {
		mMelody.GetNoteArray().InsertElem(note,index);
	    }
	    mMelody.GetNumberOfNotes()++;
	}

	void MelodyPlayer::RemoveNote(const TIndex& index)
	{
	    int nNotes = mMelody.GetNumberOfNotes();
	    if(!nNotes || index < 0 || index > nNotes-1) return;
	    TData newEndTime = TData(0.0);
	    bool modify_prior = false;
	    if(index > 0)
	    {
		newEndTime = mMelody.GetNoteArray()[index].GetTime().GetEnd();
		modify_prior = true;
	    }
	    mMelody.GetNoteArray().DeleteElem(index);
	    if(modify_prior)
	    {
		mMelody.GetNoteArray()[index-1].GetTime().SetEnd(newEndTime);
	    }
	    mMelody.GetNumberOfNotes()--;
	}

	void MelodyPlayer::SetDuration(const TData& dur)
	{
	    mDuration = dur;
	}

	void MelodyPlayer::SetAudioPtr(const Audio* audio)
	{
	    mAudioPtr = audio;
	}

	void MelodyPlayer::PlayMelody(bool play)
	{
	    mPlayMelody = play;
	}

	void MelodyPlayer::PlayAudio(bool play)
	{
	    mPlayAudio = play;
	}

    }
}

// END

