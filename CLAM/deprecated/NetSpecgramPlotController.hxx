#ifndef __NETSPECGRAMPLOTCONTROLLER__
#define __NETSPECGRAMPLOTCONTROLLER__

#include <vector>
#include "Spectrum.hxx"
#include "QtPalette.hxx"
#include "NetSpecgramRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetSpecgramPlotController : public NetPlotController 
		{
		public:
			typedef SpecgramPortMonitor MonitorType;
			NetSpecgramPlotController();
			~NetSpecgramPlotController();

			void SetData(const Spectrum& spec);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			MonitorType*                      mMonitor;
			Spectrum                          mSpec;
			std::vector< std::vector<Color> > mCachedData;
			NetSpecgramRenderer               mRenderer;
			int                               mIndex;
			int                               mSpecSize;
			QtPalette                         mPalette;
			SigSlot::Slotv0                   mSlotNewData;
			TIndex                            mLeftIndex1;
			TIndex                            mRightIndex1;
			TIndex                            mLeftIndex2;
			TIndex                            mRightIndex2;
			TIndex                            mBottomBound;
			TIndex                            mTopBound;
			TData                             mSpectralRange;
			bool                              mHasData;
			bool                              mRenderingIsDone;
			QString                           mTooltip;
	    
			void AddData(const Spectrum& spec);
			void AdaptSpectralData();
	  
			void Init(const TSize& specSize, const TData& spectralRange);

			float ClampToRange(TData value) const;

			void OnNewData();
			void ComputeIndexes();

		};
    }
}

#endif


