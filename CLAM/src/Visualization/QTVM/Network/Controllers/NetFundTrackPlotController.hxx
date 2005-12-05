#ifndef __NETFUNDTRACKPLOTCONTROLLER__
#define __NETFUNDTRACKPLOTCONTROLLER__

#include "Fundamental.hxx"
#include "NetDataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetFundTrackPlotController : public NetPlotController 
		{
		public:
			typedef FundTrackPortMonitor MonitorType;
			NetFundTrackPlotController();
			~NetFundTrackPlotController();

			void SetData(const Fundamental& fund);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			MonitorType*    mMonitor;
			DataArray       mCachedData;
			NetDataRenderer mRenderer;
			bool            mHasData;
			bool            mRenderingIsDone;
			QString         mTooltip;
			SigSlot::Slotv0 mSlotNewData;
	    
			void AddData(const TData& data);
			void Init();
			void OnNewData();

		};
    }
}

#endif


