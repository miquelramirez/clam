#include "xtime.hxx"
#include "MIDIManager.hxx"
#include "MIDIIOConfig.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIMelodyPlayer.hxx" 

namespace CLAM
{
    namespace VM
    {
	MIDIMelodyPlayer::MIDIMelodyPlayer()
	    : mMIDIDevice(""),
	      mEnqueuedDevice(""),
	      mHasEnqueuedDevice(false),
	      mMIDIProgram(0),
	      mDuration(TData(0.0))
	{
	    mMIDIMelody.AddNumberOfNotes();
	    mMIDIMelody.UpdateData();
	    mMIDIMelody.SetNumberOfNotes(0);

	    HaveData(true);
	    
	    _thread.SetThreadCode(makeMemberFunctor0((*this), MIDIMelodyPlayer, thread_code));
	}
		
	MIDIMelodyPlayer::~MIDIMelodyPlayer()	
	{
	}

	void MIDIMelodyPlayer::SetMIDIDevice(const std::string& device)
	{
	    if(!_thread.IsRunning())
	    {
		mMIDIDevice = "default:"+device;
	    }
	    else
	    {
		mEnqueuedDevice = "default:"+device;
		mHasEnqueuedDevice = true;
	    }
	}

	void MIDIMelodyPlayer::SetMIDIProgram(const int& program)
	{
	    mMIDIProgram = program;
	}
		
	void MIDIMelodyPlayer::SetData(const MIDIMelody& melody, 
				       const std::string& device, 
				       const int& program, 
				       const TData& dur)
	{
	    mMIDIMelody = melody;
	    mMIDIDevice = "default:"+device;
	    mMIDIProgram = program;
	    mDuration = dur;
	}

	MIDIMelody& MIDIMelodyPlayer::GetMIDIMelody()
	{
	    return mMIDIMelody;
	}
		
	void MIDIMelodyPlayer::thread_code()
	{
	    if(!mMIDIMelody.GetNumberOfNotes()) return;
	    
	    bool isBegin = true;
	    TIndex noteNumber = GetNoteIndex();
	    unsigned t0,t1;

	    MIDIManager manager;
	
            // program
	    MIDIIOConfig progChgCfg;
	    progChgCfg.SetDevice(mMIDIDevice.c_str());
	    progChgCfg.SetChannel(1);
	    progChgCfg.SetMessage(MIDI::eProgramChange);
	    MIDIOutControl progChg(progChgCfg);

	    // volume
	    MIDIIOConfig volCtrlCfg;
	    volCtrlCfg.SetDevice(mMIDIDevice.c_str());
	    volCtrlCfg.SetChannel(1);
	    volCtrlCfg.SetFirstData(7); 
	    volCtrlCfg.SetMessage(MIDI::eControlChange);
	    MIDIOutControl volCtrl(volCtrlCfg);

	    // note on
	    MIDIIOConfig outNoteCfg;
	    outNoteCfg.SetDevice(mMIDIDevice.c_str());
	    outNoteCfg.SetChannel(1);
	    outNoteCfg.SetMessage(MIDI::eNoteOn);
	    MIDIOutControl outNote(outNoteCfg);

	    manager.Start();

	    progChg.GetInControls().GetByNumber(0).DoControl(TControlData(mMIDIProgram)); // set program
	    volCtrl.GetInControls().GetByNumber(0).DoControl(120); // set volume
        
	    unsigned t_begin = unsigned(_time.GetBegin())*1000;
	    
	    t0 = GetTime();

	    while(true)
	    {
		t1 = GetTime()+t_begin;
		
		if(IsPaused())
		{
		    _time.SetBegin(TData(t1-t0)/TData(1000.0));
		    SetPlaying(false);
		}

		if(!IsPlaying() || (t1-t0) >= unsigned(_time.GetEnd())*1000) break;
		
		if((t1-t0) >= mMIDIMelody.GetNoteArray()[noteNumber].GetTime().GetBegin()*1000 && isBegin)
		{
		    // note on
		    outNote.GetInControls().GetByNumber(0).DoControl(TControlData(mMIDIMelody.GetNoteArray()[noteNumber].GetKey()));
		    outNote.GetInControls().GetByNumber(1).DoControl(TControlData(mMIDIMelody.GetNoteArray()[noteNumber].GetVelocity()));
		    isBegin = false;
		}
		
		if((t1-t0) >= mMIDIMelody.GetNoteArray()[noteNumber].GetTime().GetEnd()*1000 && !isBegin)
		{
		    // note off
		    outNote.GetInControls().GetByNumber(0).DoControl(TControlData(mMIDIMelody.GetNoteArray()[noteNumber].GetKey()));
		    outNote.GetInControls().GetByNumber(1).DoControl(0);
		    noteNumber++;
		    isBegin = true;
		}
	    }

	    // send panic
	    MIDIIOConfig panicCfg;
	    panicCfg.SetDevice(mMIDIDevice.c_str());
	    panicCfg.SetChannel(1);
	    panicCfg.SetFirstData(123); 
	    panicCfg.SetMessage(MIDI::eControlChange);
	    MIDIOutControl panicCtrl(panicCfg);
	    panicCtrl.GetInControls().GetByNumber(0).DoControl(0); 

	    if(!IsPaused()) _time.SetBegin(GetBeginTime());

	    if(mHasEnqueuedDevice)
	    {
		mMIDIDevice = mEnqueuedDevice;
		mHasEnqueuedDevice = false;
	    }
	       
	}

	unsigned MIDIMelodyPlayer::GetTime()
	{
	    unsigned m1,m2;
	    xtime _t;
	    xtime_get(&_t,TIME_UTC);
	    m1 = _t.sec*MILLISECONDS_PER_SECOND;
	    m2 = _t.nsec/NANOSECONDS_PER_MILLISECOND;
	    return m1+m2;
	}

	TIndex MIDIMelodyPlayer::GetNoteIndex(bool first)
	{
	    int nNotes = mMIDIMelody.GetNumberOfNotes();
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

	    if(searchValue <= mMIDIMelody.GetNoteArray()[0].GetTime().GetBegin()) return 0;
	    if(searchValue >= mMIDIMelody.GetNoteArray()[nNotes-1].GetTime().GetEnd()) return nNotes-1;
	    if(searchValue >= mMIDIMelody.GetNoteArray()[0].GetTime().GetBegin() &&
	       searchValue <= mMIDIMelody.GetNoteArray()[0].GetTime().GetEnd()) return 0;
	    if(searchValue >= mMIDIMelody.GetNoteArray()[nNotes-1].GetTime().GetBegin() &&
	       searchValue <= mMIDIMelody.GetNoteArray()[nNotes-1].GetTime().GetEnd())
		return nNotes-1;

	    TIndex index = 0;
	    TIndex currentIndex = 0;
	    TIndex left_index = 0;
	    TIndex right_index = nNotes-1;
	    while(left_index <= right_index)
	    {
		currentIndex = (left_index+right_index)/2;
		if(searchValue >= mMIDIMelody.GetNoteArray()[currentIndex].GetTime().GetBegin() &&
		   searchValue <= mMIDIMelody.GetNoteArray()[currentIndex].GetTime().GetEnd())
		{
		    index=currentIndex;
		    break;
		}
		if(searchValue < mMIDIMelody.GetNoteArray()[currentIndex].GetTime().GetBegin())
		{
		    right_index = currentIndex-1;
		}
		else if(searchValue > mMIDIMelody.GetNoteArray()[currentIndex].GetTime().GetBegin())
		{
		    left_index = currentIndex+1;
		}
	    }
	    return index;
	}

	void MIDIMelodyPlayer::UpdateNoteKey(const TIndex& index, const int& newKey)
	{
	    mMIDIMelody.GetNoteArray()[index].SetKey(newKey);
	}

	void MIDIMelodyPlayer::UpdateNoteDuration(const TIndex& index, const TData& beginTime)
	{
	    mMIDIMelody.GetNoteArray()[index].GetTime().SetBegin(beginTime);
	    if(index > 0) mMIDIMelody.GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
	}

	void MIDIMelodyPlayer::AddNote(const TIndex& index, const TData& beginTime, const int& key)
	{
	    bool addElem = false;
	    MIDINote midiNote;
	    MediaTime time;
	    time.SetBegin(beginTime);
	    if(mMIDIMelody.GetNumberOfNotes())
	    {
		if(index >= mMIDIMelody.GetNumberOfNotes())
		{
		    time.SetEnd(mMIDIMelody.GetNoteArray()[mMIDIMelody.GetNumberOfNotes()-1].GetTime().GetEnd());
		    addElem = true;
		}
		else
		{
		    time.SetEnd(mMIDIMelody.GetNoteArray()[index].GetTime().GetBegin());
		}
	    }
	    else
	    {
		time.SetEnd(mDuration);
		_time.SetEnd(time.GetEnd());
	    }

	    if(index > 0 && mMIDIMelody.GetNumberOfNotes())
	    {
		 if(addElem)
		 {
		     mMIDIMelody.GetNoteArray()[mMIDIMelody.GetNumberOfNotes()-1].GetTime().SetEnd(beginTime); 
		 }
		 else
		 {
		     mMIDIMelody.GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
		 }
	    }

	    midiNote.SetKey(key);
	    midiNote.SetVelocity(120);
	    midiNote.SetTime(time);

	    if(addElem || !mMIDIMelody.GetNumberOfNotes())
	    {
		mMIDIMelody.GetNoteArray().AddElem(midiNote);
	    }
	    else
	    {
		mMIDIMelody.GetNoteArray().InsertElem(midiNote,index);
	    }
  
	    mMIDIMelody.GetNumberOfNotes()++;
       
	}

	void MIDIMelodyPlayer::RemoveNote(const TIndex& index)
	{
	    TData newEndTime = TData(0.0);
	    bool modify_prior = false;
	    if(index > 0)
	    {
		newEndTime = mMIDIMelody.GetNoteArray()[index].GetTime().GetEnd();
		modify_prior = true;
	    }
	    mMIDIMelody.GetNoteArray().DeleteElem(index);
	    if(modify_prior)
	    {
		mMIDIMelody.GetNoteArray()[index-1].GetTime().SetEnd(newEndTime);
	    }
	    mMIDIMelody.GetNumberOfNotes()--;
	}

	void MIDIMelodyPlayer::SetDuration(const TData& dur)
	{
	    mDuration = dur;
	}

    }
}


// END

