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
	    : mSampleRate(TData(44100.0)),
	      mDuration(TData(0.0))
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

	    if(mMelody.GetNumberOfNotes())
	    {
		MediaTime time;
		time.SetBegin(TData(0.0));
		time.SetEnd(mMelody.GetNoteArray()[mMelody.GetNumberOfNotes()-1].GetTime().GetEnd());
		SetBounds(time);
	    }
	}

	Melody& MelodyPlayer::GetMelody()
	{
	    return mMelody;
	}
		
	void MelodyPlayer::thread_code()
	{
	    if(!mMelody.GetNumberOfNotes()) return;

	    TSize frameSize = 512;                    

	    AudioManager manager((int)mSampleRate,(int)frameSize);  
	    AudioOut channel;   
	    AudioIOConfig audioOutCfg;     
	    audioOutCfg.SetChannelID(0);    
	    channel.Configure(audioOutCfg); 
			
	    AudioManager::Current().Start();                            
	    channel.Start();  

	    SimpleOscillatorConfig oscCfg;
	    oscCfg.SetSamplingRate(mSampleRate);
	    oscCfg.SetAmplitude(TData(0.6));
	    SimpleOscillator osc(oscCfg);
			
	    InControl& freqControl = osc.GetInControls().Get("Pitch");
    
	    Audio samples;                
	    samples.SetSize(frameSize);
				
	    TIndex firstIndex=GetNoteIndex();
	    TIndex lastIndex=GetNoteIndex(false);
	    TIndex k=firstIndex;

	    TIndex start = int(_time.GetBegin()*mSampleRate);
	    int nSamples = start+int((mMelody.GetNoteArray()[lastIndex].GetTime().GetEnd()-mMelody.GetNoteArray()[firstIndex].GetTime().GetBegin())*mSampleRate);
	    
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

		if(TData(i/mSampleRate) >= mMelody.GetNoteArray()[k].GetTime().GetBegin() &&
		   TData(i/mSampleRate) < mMelody.GetNoteArray()[k].GetTime().GetEnd())
		{
		    freqControl.DoControl(mMelody.GetNoteArray()[k].GetFundFreq());
		}
		else
		{
		    freqControl.DoControl(0);
		}
		osc.Do(samples);
		channel.Do(samples);
	    }
	    osc.Stop();
	    channel.Stop(); 
	    if(!IsPaused()) _time.SetBegin(GetBeginTime());
	}

	TIndex MelodyPlayer::GetNoteIndex(bool first)
	{
	    if(mMelody.GetNumberOfNotes()==1)
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
	    TIndex index = 0;
	    for(TIndex i=0; i < mMelody.GetNumberOfNotes(); i++)
	    {
		if(searchValue >= mMelody.GetNoteArray()[i].GetTime().GetBegin() &&
		   searchValue <= mMelody.GetNoteArray()[i].GetTime().GetEnd())
		{
		    index=i;
		    break;
		}
	    }
	    return index;
	}

	void MelodyPlayer::UpdateNotePitch(const TIndex& index, const TData& newPitch)
	{
	    mMelody.GetNoteArray()[index].SetFundFreq(newPitch);
	}

	void MelodyPlayer::UpdateNoteDuration(const TIndex& index, const TData& beginTime)
	{
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

    }
}

// END

