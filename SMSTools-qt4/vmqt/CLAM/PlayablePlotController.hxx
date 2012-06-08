#ifndef __PLAYABLEPLOTCONTROLLER__
#define __PLAYABLEPLOTCONTROLLER__

#include <CLAM/MediaTime.hxx>
#include <CLAM/RegionMarker.hxx>
#include <CLAM/PlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		class PlayablePlotController : public PlotController
		{
			Q_OBJECT

		public:
			PlayablePlotController();
			virtual ~PlayablePlotController();

			virtual void SetSelPos(double value, bool render);
			virtual void Draw();
			void SetRegionColor(Color c);
			bool IsPlayable();
	    
		signals:
			void selectedRegion(MediaTime);
			void currentPlayingTime(float);
			void stopPlayingTime(float);
			void startPlaying();
			void stopPlaying();

		public slots:
			virtual void setHBounds(double, double)=0;
			virtual void setVBounds(double, double)=0;
			virtual void setSelectedXPos(double)=0;
			
			void updateTimePos(float);
			void stopPlaying(float);
			
		protected:
			virtual void SetHBounds(double left, double right);
			virtual void SetVBounds(double bottom, double top);

			bool CanDrawSelectedPos();

			void SetSampleRate(double sr);
			void SetDuration(double dur);
			double GetSampleRate() const;
			double GetDuration() const;

			virtual void FullView()=0;

		private:
			RegionMarker mRegionMarker;
			double       mSampleRate;
			double       mDuration;
			bool         mIsPlaying;
				
			double GetBeginRegion() const;
			double GetEndRegion() const;
			void SetBeginRegion(double value);
			void SetEndRegion(double value);

			MediaTime GetRegionTime() const;
	    
		};
    }
}

#endif

