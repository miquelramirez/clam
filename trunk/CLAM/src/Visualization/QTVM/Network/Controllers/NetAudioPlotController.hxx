#ifndef __NETAUDIOPLOTCONTROLLER__
#define __NETAUDIOPLOTCONTROLLER__

#include "Audio.hxx"
#include "NetDataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioPlotController : public NetPlotController
		{
		public:
			typedef AudioPortMonitor MonitorType;
			NetAudioPlotController();
			~NetAudioPlotController();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);
			void UpdatePoint(const TData& x, const TData& y);
		    
		protected:
			void FullView();

		private:
			Audio           mCacheData;
			NetDataRenderer mRenderer;
			MonitorType*    mMonitor;
			bool            mHasData;
			bool            mRenderingIsDone;
			QString         mTooltip;

			void Init(const TSize& frameSize);
		};
	}
}

#endif

