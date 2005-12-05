#ifndef __NETAUDIOBUFFPLOTCONTROLLER__
#define __NETAUDIOBUFFPLOTCONTROLLER__

#include "Audio.hxx"
#include "NetDataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetAudioBuffPlotController : public NetPlotController 
		{
		public:
			typedef AudioBuffPortMonitor MonitorType;
			NetAudioBuffPlotController();
			~NetAudioBuffPlotController();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);
	
		protected:
			void FullView();

		private:
			MonitorType*     mMonitor;
			DataArray        mCachedData;
			NetDataRenderer  mRenderer;
			TSize            mFrameSize;
			bool             mHasData;
			QString          mTooltip;
			bool             mRenderingIsDone;
			SigSlot::Slotv0  mSlotNewData;

			void AddData(const DataArray& data);
	  
			void Init(const TSize& frameSize);
			void OnNewData();
		};
    }
}

#endif


