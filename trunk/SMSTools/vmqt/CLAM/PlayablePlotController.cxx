#include <CLAM/PlayablePlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		PlayablePlotController::PlayablePlotController()
			: mSampleRate(44100.0)
			, mDuration(0.0)
			, mIsPlaying(false)
		{
		}
		
		PlayablePlotController::~PlayablePlotController()
		{
		}

		void PlayablePlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(IsKeyShiftPressed())
				{
					if(value > GetBeginRegion() && value < GetEndRegion())
					{
						if(value > GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/2.0))
						{
							SetEndRegion(value);
						}
						else
						{
							SetBeginRegion(value);
						}
					}
					else
					{
						if(value > GetEndRegion())
						{
							SetEndRegion(value);
						}
						if(value < GetBeginRegion())
						{
							SetBeginRegion(value);
						}
					}
					UpdateDial(value);
					PlotController::SetSelPos(GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/2.0),false);
				}
				else
				{
					PlotController::SetSelPos(value, true);
					SetBeginRegion(GetSelPos());
					SetEndRegion(GetSelPos());
				}
				emit selectedRegion(GetRegionTime());
			}
		}
		
		void PlayablePlotController::Draw()
		{
			PlotController::Draw();
			mRegionMarker.Render();
		}

		void PlayablePlotController::SetRegionColor(Color c)
		{
			mRegionMarker.SetColor(c);
		}
		
		void PlayablePlotController::SetHBounds(double left, double right)
		{
			PlotController::SetHBounds(left,right);
			mRegionMarker.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void PlayablePlotController::SetVBounds(double bottom, double top)
		{
			PlotController::SetVBounds(bottom,top);
			mRegionMarker.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void PlayablePlotController::SetBeginRegion(double value)
		{
			mRegionMarker.SetBegin(value);
		}

		void PlayablePlotController::SetEndRegion(double value)
		{
			mRegionMarker.SetEnd(value);
		}

		double PlayablePlotController::GetBeginRegion() const
		{
			return mRegionMarker.GetBegin();
		}

		double PlayablePlotController::GetEndRegion() const
		{
			return mRegionMarker.GetEnd();
		}

		MediaTime PlayablePlotController::GetRegionTime() const
		{
			MediaTime time;
			time.SetBegin(GetBeginRegion()/mSampleRate);
			time.SetEnd(GetEndRegion()/mSampleRate);
			if(time.GetBegin() < time.GetEnd())
			{
				time.AddDuration();
				time.UpdateData();
				time.SetDuration(time.GetEnd()-time.GetBegin());
			}
			else
			{
				time.SetEnd(mDuration);
			}
			return time;
		}

		void PlayablePlotController::updateTimePos(float time)
		{
			if(mIsPlaying && time<=GetDialPos()/mSampleRate) return;
			if(!mIsPlaying) 
			{
				mIsPlaying =true;
				emit startPlaying();
			}
			UpdateDial(time*mSampleRate);
			PlotController::PlayingPos(time*mSampleRate);
			emit currentPlayingTime(time);
		}

		void PlayablePlotController::stopPlaying(float time)
		{
			if(!mIsPlaying) return;
			mIsPlaying=false;
			emit stopPlayingTime(time);
			UpdateDial(time*mSampleRate);
			emit stopPlaying();
		}

		bool PlayablePlotController::CanDrawSelectedPos()
		{
			return (!mIsPlaying && PlotController::CanDrawSelectedPos());
		}

		bool PlayablePlotController::IsPlayable()
		{
			return true;
		}

		void PlayablePlotController::SetSampleRate(double sr)
		{
			mSampleRate = sr;
		}

		void PlayablePlotController::SetDuration(double dur)
		{
			mDuration = dur;
		}

		double PlayablePlotController::GetSampleRate() const
		{
			return mSampleRate;
		}

		double PlayablePlotController::GetDuration() const
		{
			return mDuration;
		}

    }    
}

// END

