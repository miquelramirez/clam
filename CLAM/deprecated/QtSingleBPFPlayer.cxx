#include "Message.hxx"
#include "BPFPlayer.hxx"
#include "QtSingleBPFPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		const TData min_ref = 8.1758;  // 8.1758 Hz 
		const TData max_ref = 12545.9; // 12545.9 Hz 

		QtSingleBPFPlayer::QtSingleBPFPlayer(QWidget* parent)
			: QtSinglePlayer(parent) 
			, mYMin(0.0)
			, mYMax(1.0)
			, mMustDoMapping(false)
			, mWorking(false)
			, mCancelThread(false)
		{
			SetPlayer(new BPFPlayer());
			mThread.SetThreadCode(makeMemberFunctor0((*this), QtSingleBPFPlayer, thread_code));
		}

		QtSingleBPFPlayer::~QtSingleBPFPlayer()
		{
			StopThread();
		}

		void QtSingleBPFPlayer::SetData(const BPF& bpf)
		{
			std::pair<double,double> yrange = GetMinMaxY(bpf);
			mYMin = yrange.first;
			mYMax = yrange.second;
		    mMustDoMapping =  (mYMin >= min_ref && mYMax <= max_ref) ? false : true;
			if(!mMustDoMapping) 
			{
				((BPFPlayer*)mPlayer)->SetData(bpf);
				return;
			}
			mBPF = bpf;
			AdaptIncomingData();
		}

		void QtSingleBPFPlayer::SetAudioPtr(const Audio* audio, int chn)
		{
			((BPFPlayer*)mPlayer)->SetAudioPtr(audio,chn);
		}

		void QtSingleBPFPlayer::SetDuration(TData duration)
		{
			((BPFPlayer*)mPlayer)->SetDuration(duration);
		}

		void QtSingleBPFPlayer::SetSampleRate(TData sr)
		{
			((BPFPlayer*)mPlayer)->SetSampleRate(sr);
		}

		void QtSingleBPFPlayer::Update(TIndex index, TData value)
		{
			if(mMustDoMapping) value = (value-mYMin)*(max_ref-min_ref)/(mYMax-mYMin)+min_ref;
			((BPFPlayer*)mPlayer)->Update(index,value);
		}

		void QtSingleBPFPlayer::play()
		{
			if(mWorking)
			{
				CLAM::VM::Message(QMessageBox::Information,
								  "BPFPlayer",
								  "CLAM::VM::BPFPlayer::play() Please wait for adapting data.");
				return;
			}
			QtSinglePlayer::play();
		}

		void QtSingleBPFPlayer::AdaptIncomingData()
		{
			StopThread();
			mCancelThread = false;
			mThread.Start();
		}

		void QtSingleBPFPlayer::thread_code()
		{
			mWorking = true;
			for(TIndex i=0; i < mBPF.Size(); i++)
			{
				if(mCancelThread) break;
				TData value = mBPF.GetValueFromIndex(i);
				value = (value-mYMin)*(max_ref-min_ref)/(mYMax-mYMin)+min_ref;
				mBPF.SetValue(i,value);
			}
			if(!mCancelThread) ((BPFPlayer*)mPlayer)->SetData(mBPF);
			mWorking = false;
		}

		void QtSingleBPFPlayer::StopThread()
		{
			if(!mThread.IsRunning()) return;
			mCancelThread = true;
			mThread.Stop();
		}
		
		std::pair<double,double> QtSingleBPFPlayer::GetMinMaxY(const BPF& bpf)
		{
			double min_value=1E9;
			double max_value=-1E9;
			for(TIndex i=0; i < bpf.Size(); i++)
			{
				double current = double(bpf.GetValueFromIndex(i));
				if(current > max_value)
				{
					max_value = current;
				}
				else if(current < min_value)
				{
					min_value = current;
				}
			}
			return std::make_pair(min_value, max_value);
		}
	}
}

// END

