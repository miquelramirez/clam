#ifndef __NETSINTRACKSPLOTCONTROLLER__
#define __NETSINTRACKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetSinTracksRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"
#include "SinTracking.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetSinTracksPlotController : public NetPlotController 
		{
		public:
			typedef SinTracksPortMonitor MonitorType;
			NetSinTracksPlotController();
			~NetSinTracksPlotController();

			void SetData(const SpectralPeakArray& peaks);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			MonitorType*             mMonitor;
			Array<SpectralPeakArray> mPeakMtx;
			NetSinTracksRenderer     mRenderer;
			SigSlot::Slotv0          mSlotNewData;
			SinTracking              mSinTracking;
			int                      mIndex;
			TIndex                   mLeftIndex1;
			TIndex                   mRightIndex1;
			TIndex                   mLeftIndex2;
			TIndex                   mRightIndex2;
			bool                     mHasData;
			bool                     mRenderingIsDone;
			QString                  mTooltip;

			void AddData(const SpectralPeakArray& data);
	  
			void Init();
			void OnNewData();
			void ComputeIndexes();

		};
    }
}

#endif


