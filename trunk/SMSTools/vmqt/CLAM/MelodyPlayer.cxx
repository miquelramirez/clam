#include <iostream>
#include <CLAM/AudioIO.hxx> 
#include <CLAM/AudioManager.hxx> 
#include <CLAM/AudioOut.hxx> 
#include <CLAM/SimpleOscillator.hxx> 
#include <CLAM/MelodyPlayer.hxx> 

namespace CLAM
{
    namespace VM
    {
		MelodyPlayer::MelodyPlayer()
			: mCurrent("default")
			, mAudioPtr(0)
			, mSampleRate(TData(44100.0))
			, mDuration(TData(0.0))
			, mPlayMelody(true)
			, mPlayAudio(false)
		{
			InitTables();
			HaveData(true);

			mThread.SetThreadCode(makeMemberFunctor0((*this), MelodyPlayer, thread_code));
		}
		
		MelodyPlayer::~MelodyPlayer()
		{
		}
		
		void MelodyPlayer::AddData(const std::string& key, const Melody& melody, const TData& dur)
		{
			AddMelody(key,melody);
			mDuration = dur;
		}

		Melody& MelodyPlayer::GetMelody(const std::string& key)
		{
			bool haveKey = HaveKey(key);
			unsigned index = (haveKey) ? GetMelodyIndex(key) : GetMelodyIndex(mCurrent);
			if(!haveKey)
			{
				std::cout << "WARNING: MelodyPlayer::GetMelody(const std::string& key)" << std::endl;
				std::cout << key << " not exist, returning current melody";
			}
			return mMelodies[index];
		}
		
		void MelodyPlayer::thread_code()
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			if(!mMelodies[melodyIndex].GetNumberOfNotes()) return;
		
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
			
			
			manager.Start();                            
			channelL.Start();  
			channelR.Start();

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(mSampleRate);
			oscCfg.SetAmplitude(TData(0.6));
			SimpleOscillator osc(oscCfg);
			
			Audio samplesL;             
			Audio samplesR;
			samplesL.SetSize(frameSize);
			samplesR.SetSize(frameSize);
				
			TIndex firstIndex = GetNoteIndex();
			TIndex lastIndex = GetNoteIndex(false);
			TIndex k = firstIndex;

			TIndex start = int(mTime.GetBegin()*mSampleRate);
			int nSamples = int(mMelodies[melodyIndex].GetNoteArray()[lastIndex].GetTime().GetEnd()*mSampleRate);
	    
			TIndex leftIndex = start;        
			TIndex rightIndex = leftIndex+frameSize;

			osc.Start();

			for(TIndex i=start; i < nSamples; i+=frameSize)
			{
				if(IsPaused())
				{
					mTime.SetBegin(TData(i)/mSampleRate);
					SetPlaying(false);
				}
				if(!IsPlaying()) break;

				if(TData(i/mSampleRate) >= mMelodies[melodyIndex].GetNoteArray()[k].GetTime().GetEnd()) k++;

				if(mPlayMelody)
				{
					if(TData(i/mSampleRate) >= mMelodies[melodyIndex].GetNoteArray()[k].GetTime().GetBegin() &&
					   TData(i/mSampleRate) < mMelodies[melodyIndex].GetNoteArray()[k].GetTime().GetEnd())
					{
						SendFloatToInControl(osc,"Pitch",mMelodies[melodyIndex].GetNoteArray()[k].GetFundFreq());
					}
					else
					{
						SendFloatToInControl(osc,"Pitch",0);
					}
				}
				else
				{
					SendFloatToInControl(osc,"Pitch",0);
				}

				if(mPlayMelody && mPlayAudio)
				{
					osc.Do(samplesL);
					if(mAudioPtr) 
					{
						if(rightIndex < mAudioPtr->GetSize())
						{
							mAudioPtr->GetAudioChunk(leftIndex,rightIndex,samplesR);
						}
					}
				}		
				else
				{ 
					if(mPlayAudio)
					{
						if(mAudioPtr) 
						{
							if(rightIndex < mAudioPtr->GetSize())
							{
								mAudioPtr->GetAudioChunk(leftIndex,rightIndex,samplesL);
							}
						}
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

				mSigPlayingTime.Emit(TData(leftIndex)/mSampleRate);

				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			osc.Stop();
			channelL.Stop(); 
			channelR.Stop();
			if(!IsPaused()) mTime.SetBegin(GetBeginTime());

			TData stopTime = (IsStopped()) ? TData(leftIndex)/mSampleRate : (IsPaused()) ? mTime.GetBegin() : mTime.GetEnd();
			mSigStop.Emit(stopTime);
		}

		TIndex MelodyPlayer::GetNoteIndex(bool first)
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

		void MelodyPlayer::UpdateNotePitch(const TIndex& index, const TData& newPitch)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(!nNotes || index < 0 || index > nNotes-1) return;
			mMelodies[melodyIndex].GetNoteArray()[index].SetFundFreq(newPitch);
		}

		void MelodyPlayer::UpdateNoteDuration(const TIndex& index, const TData& beginTime)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			int nNotes = mMelodies[melodyIndex].GetNumberOfNotes();
			if(!nNotes || index < 0 || index > nNotes-1) return;
			mMelodies[melodyIndex].GetNoteArray()[index].GetTime().SetBegin(beginTime);
			if(index > 0) mMelodies[melodyIndex].GetNoteArray()[index-1].GetTime().SetEnd(beginTime);
		}

		void MelodyPlayer::AddNote(const TIndex& index, const TData& beginTime, const TData& pitch)
		{
			unsigned melodyIndex = GetMelodyIndex(mCurrent);
			bool addElem = false;
			Note note;
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
	    
			note.SetFundFreq(pitch);
			note.SetTime(time);

			if(addElem || !mMelodies[melodyIndex].GetNumberOfNotes())
			{
				mMelodies[melodyIndex].GetNoteArray().AddElem(note);
			}
			else
			{
				mMelodies[melodyIndex].GetNoteArray().InsertElem(note,index);
			}
			mMelodies[melodyIndex].GetNumberOfNotes()++;
		}

		void MelodyPlayer::RemoveNote(const TIndex& index)
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

		void MelodyPlayer::SetCurrent(const std::string& current)
		{
			if(!HaveKey(current)) return;
			mCurrent = current;
		}

		void MelodyPlayer::InitTables()
		{
			mIndexTable["default"]=0;
			mMelodies.resize(1);
			mMelodies[0].AddNumberOfNotes();
			mMelodies[0].UpdateData();
			mMelodies[0].SetNumberOfNotes(0);
		}

		void MelodyPlayer::AddMelody(const std::string& key, const Melody& melody)
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
		
		bool MelodyPlayer::HaveKey(const std::string& key)
		{
			return mIndexTable.find(key) != mIndexTable.end();
		}

		unsigned  MelodyPlayer::GetMelodyIndex(const std::string& key)
		{
			return mIndexTable[key];
		}

    }
}

// END

