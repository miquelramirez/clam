#include <iostream>
#include <typeinfo>
#include <CLAM/xtime.hxx>
#include <CLAM/MIDIManager.hxx>
#include <CLAM/MIDIIOConfig.hxx>
#include <CLAM/MIDIOutControl.hxx>
#include <CLAM/MIDIMelodyPlayer.hxx> 

namespace CLAM
{
    namespace VM
    {
		MIDIMelodyPlayer::MIDIMelodyPlayer()
			: mCurrent("default")
			, mMIDIDevice("")
			, mEnqueuedDevice("")
			, mHasEnqueuedDevice(false)
			, mMIDIProgram(0)
			, mDuration(TData(0.0))
		{
			InitTables();
			HaveData(true);
	    
			mThread.SetThreadCode(makeMemberFunctor0((*this), MIDIMelodyPlayer, thread_code));
		}
		
		MIDIMelodyPlayer::~MIDIMelodyPlayer()	
		{
		}

		void MIDIMelodyPlayer::SetMIDIDevice(const std::string& device)
		{
			if(!mThread.IsRunning())
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
		
		void MIDIMelodyPlayer::AddData(const std::string& key, const MIDIMelody& melody, const TData& dur)
		{
			AddMelody(key,melody);
			mDuration = dur;
		}

		MIDIMelody& MIDIMelodyPlayer::GetMIDIMelody(const std::string& key)
		{
			bool haveKey = HaveKey(key);
			unsigned index = (haveKey) ? GetMelodyIndex(key) : GetMelodyIndex(mCurrent);
			if(!haveKey)
			{
				std::cout << "WARNING: MIDIMelodyPlayer::GetMelody(const std::string& key)" << std::endl;
				std::cout << key << " not exist, returning current melody";
			}
			return mMelodies[index];
		}
		
		void MIDIMelodyPlayer::thread_code()
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			if(!mMelodies[melodyIndex].GetNumberOfNotes()) return;
	    
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

			SendFloatToInControl(progChg,0,TControlData(mMIDIProgram)); // set program
			SendFloatToInControl(volCtrl,0,120); // set volume
        
			TData stopTime;

			unsigned playing_t0 = GetTime();
			unsigned playing_t1;

			unsigned t_begin = unsigned(mTime.GetBegin()*TData(1000.0));

			t0 = GetTime();
	    
			while(true)
			{
				playing_t1 = GetTime();

				t1 = GetTime()+t_begin;

				if(IsPaused())
				{
					mTime.SetBegin(TData(t1-t0)/TData(1000.0));
					SetPlaying(false);
				}

				if(!IsPlaying() || (t1-t0) >= unsigned(mTime.GetEnd()*TData(1000.0))) 
				{
					stopTime = TData(t1-t0)/TData(1000.0);
					break;
				}

				if((t1-t0) >= mMelodies[melodyIndex].GetNoteArray()[noteNumber].GetTime().GetBegin()*1000 && isBegin)
				{
					// note on
					SendFloatToInControl(outNote,0,TControlData(mMelodies[melodyIndex].GetNoteArray()[noteNumber].GetKey()));
					SendFloatToInControl(outNote,1,TControlData(mMelodies[melodyIndex].GetNoteArray()[noteNumber].GetVelocity()));
					isBegin = false;
				}
		
				if((t1-t0) >= mMelodies[melodyIndex].GetNoteArray()[noteNumber].GetTime().GetEnd()*1000 && !isBegin)
				{
					// note off
					SendFloatToInControl(outNote,0,TControlData(mMelodies[melodyIndex].GetNoteArray()[noteNumber].GetKey()));
					SendFloatToInControl(outNote,1,0);
					noteNumber++;
					isBegin = true;
				}
		
				if(playing_t1-playing_t0 >= 110)
				{
					mSigPlayingTime.Emit(TData(t1-t0)/TData(1000.0));
					playing_t0 = GetTime();
				}
			}
	    
			if(IsPaused()) stopTime = mTime.GetBegin(); else if(!IsStopped()) stopTime = mTime.GetEnd();
	    
			mSigStop.Emit(stopTime);

			// send panic
			MIDIIOConfig panicCfg;
			panicCfg.SetDevice(mMIDIDevice.c_str());
			panicCfg.SetChannel(1);
			panicCfg.SetFirstData(123); 
			panicCfg.SetMessage(MIDI::eControlChange);
			MIDIOutControl panicCtrl(panicCfg);
			SendFloatToInControl(panicCtrl,0,0);

			if(!IsPaused()) mTime.SetBegin(GetBeginTime());

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
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(nNotes==1)
			{
				return 0;
			}
			TData searchValue = (first) ? mTime.GetBegin() : mTime.GetEnd();
			if(searchValue <= mMelodies[melodyIndex].GetNoteArray()[0].GetTime().GetBegin()) return 0;
			if(searchValue >= mMelodies[melodyIndex].GetNoteArray()[nNotes-1].GetTime().GetEnd()) return nNotes-1;
			if(searchValue >= mMelodies[melodyIndex].GetNoteArray()[0].GetTime().GetBegin() &&
			   searchValue <= mMelodies[melodyIndex].GetNoteArray()[0].GetTime().GetEnd()) return 0;
			if(searchValue >= mMelodies[melodyIndex].GetNoteArray()[nNotes-1].GetTime().GetBegin() &&
			   searchValue <= mMelodies[melodyIndex].GetNoteArray()[nNotes-1].GetTime().GetEnd())
				return nNotes-1;

			TIndex index = 0;
			TIndex currentIndex = 0;
			TIndex left_index = 0;
			TIndex right_index = nNotes-1;
			while(left_index <= right_index)
			{
				currentIndex = (left_index+right_index)/2;
				if(searchValue >= mMelodies[melodyIndex].GetNoteArray()[currentIndex].GetTime().GetBegin() &&
				   searchValue <= mMelodies[melodyIndex].GetNoteArray()[currentIndex].GetTime().GetEnd())
				{
					index=currentIndex;
					break;
				}
				if(searchValue < mMelodies[melodyIndex].GetNoteArray()[currentIndex].GetTime().GetBegin())
				{
					right_index = currentIndex-1;
				}
				else if(searchValue > mMelodies[melodyIndex].GetNoteArray()[currentIndex].GetTime().GetBegin())
				{
					left_index = currentIndex+1;
				}
			}
			return index;
		}

		void MIDIMelodyPlayer::UpdateNoteKey(const TIndex& index, const int& newKey)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(!nNotes || index < 0 || index > nNotes-1) return;
			mMelodies[melodyIndex].GetNoteArray()[index].SetKey(newKey);
		}

		void MIDIMelodyPlayer::UpdateNoteDuration(const TIndex& index, const TData& beginTime)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(!nNotes || index < 0 || index > nNotes-1) return;
			mMelodies[melodyIndex].GetNoteArray()[index].GetTime().SetBegin(beginTime);
			if(index > 0) mMelodies[melodyIndex].GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
		}

		void MIDIMelodyPlayer::AddNote(const TIndex& index, const TData& beginTime, const int& key)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			bool addElem = false;
			MIDINote midiNote;
			MediaTime time;
			time.SetBegin(beginTime);
			if(mMelodies[melodyIndex].GetNumberOfNotes())
			{
				if(index >= mMelodies[melodyIndex].GetNumberOfNotes())
				{
					time.SetEnd(mMelodies[melodyIndex].GetNoteArray()[mMelodies[melodyIndex].GetNumberOfNotes()-1].GetTime().GetEnd());
					addElem = true;
				}
				else
				{
					time.SetEnd(mMelodies[melodyIndex].GetNoteArray()[index].GetTime().GetBegin());
				}
			}
			else
			{
				time.SetEnd(mDuration);
				mTime.SetEnd(time.GetEnd());
			}

			if(index > 0 && mMelodies[melodyIndex].GetNumberOfNotes())
			{
				if(addElem)
				{
					mMelodies[melodyIndex].GetNoteArray()[mMelodies[melodyIndex].GetNumberOfNotes()-1].GetTime().SetEnd(beginTime); 
				}
				else
				{
					mMelodies[melodyIndex].GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
				}
			}

			midiNote.SetKey(key);
			midiNote.SetVelocity(120);
			midiNote.SetTime(time);

			if(addElem || !mMelodies[melodyIndex].GetNumberOfNotes())
			{
				mMelodies[melodyIndex].GetNoteArray().AddElem(midiNote);
			}
			else
			{
				mMelodies[melodyIndex].GetNoteArray().InsertElem(midiNote,index);
			}
  
			mMelodies[melodyIndex].GetNumberOfNotes()++;
       
		}

		void MIDIMelodyPlayer::RemoveNote(const TIndex& index)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(!nNotes || index < 0 || index > nNotes-1) return;
			TData newEndTime = TData(0.0);
			bool modify_prior = false;
			if(index > 0)
			{
				newEndTime = mMelodies[melodyIndex].GetNoteArray()[index].GetTime().GetEnd();
				modify_prior = true;
			}
			mMelodies[melodyIndex].GetNoteArray().DeleteElem(index);
			if(modify_prior)
			{
				mMelodies[melodyIndex].GetNoteArray()[index-1].GetTime().SetEnd(newEndTime);
			}
			mMelodies[melodyIndex].GetNumberOfNotes()--;
		}

		void MIDIMelodyPlayer::SetDuration(const TData& dur)
		{
			mDuration = dur;
		}

		void MIDIMelodyPlayer::SetCurrent(const std::string& current)
		{
			if(!HaveKey(current)) return;
			mCurrent = current;
		}

		void MIDIMelodyPlayer::InitTables()
		{
			mIndexTable["default"]=0;
			mMelodies.resize(1);
			mMelodies[0].AddNumberOfNotes();
			mMelodies[0].UpdateData();
			mMelodies[0].SetNumberOfNotes(0);
		}

		void MIDIMelodyPlayer::AddMelody(const std::string& key, const MIDIMelody& melody)
		{
			unsigned index = 0;
			bool haveKey = false;
			if(HaveKey(key)) 
			{
				index = GetMelodyIndex(key);
				haveKey = true;
			}
			if(haveKey)
			{
				mMelodies[index] = melody;
			}
			else
			{
				unsigned currentSize = mMelodies.size();
				mIndexTable[key] = currentSize;
				mMelodies.resize(currentSize+1);
				mMelodies[currentSize] = melody;
			}
		}

		bool MIDIMelodyPlayer::HaveKey(const std::string& key)
		{
			return mIndexTable.find(key) != mIndexTable.end();
		}

		unsigned MIDIMelodyPlayer::GetMelodyIndex(const std::string& key)
		{
			return mIndexTable[key];
		}

    }
}

// END

