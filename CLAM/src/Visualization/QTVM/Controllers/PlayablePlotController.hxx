#ifndef __PLAYABLEPLOTCONTROLLER__
#define __PLAYABLEPLOTCONTROLLER__

#include "MediaTime.hxx"
#include "RegionMarker.hxx"
#include "PlotController.hxx"

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

			virtual void SetSelPos(const double& value, bool render);
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
			virtual void setSelectedXPos(double)=0;
			
			void updateTimePos(float);
			void stopPlaying(float);
			
		protected:
			virtual void SetHBounds(const double& left,const double& right);
			virtual void SetVBounds(const double& bottom,const double& top);

			bool CanDrawSelectedPos();

			void SetSampleRate(const double& sr);
			void SetDuration(const double& dur);
			const double& GetSampleRate() const;
			const double& GetDuration() const;

			virtual void FullView()=0;

		private:
			RegionMarker mRegionMarker;
			double       mSampleRate;
			double       mDuration;
			bool         mIsPlaying;
				
			const double& GetBeginRegion() const;
			const double& GetEndRegion() const;
			void SetBeginRegion(const double& value);
			void SetEndRegion(const double& value);

			MediaTime GetRegionTime() const;
	    
		};
    }
}

#endif

